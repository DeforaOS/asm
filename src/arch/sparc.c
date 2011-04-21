/* $Id$ */
/* Copyright (c) 2011 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel as */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <stddef.h>
#include "Asm.h"


/* sparc */
/* private */
/* variables */
static ArchDescription _sparc_description = { "elf", ARCH_ENDIAN_BIG, 32, 32 };

#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _sparc_registers[] =
{
#include "sparc.reg"
	{ NULL,		0, 0 }
};

static ArchInstruction _sparc_instructions[] =
{
#include "sparc.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
/* plug-in */
static int _sparc_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


/* protected */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"sparc",
	&_sparc_description,
	_sparc_registers,
	_sparc_instructions,
	_sparc_write,
	NULL
};


/* private */
/* functions */
/* plug-in */
/* sparc_write */
static int _write_branch(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode);
static int _write_integer(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode);
static int _write_loadstore(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode);
static int _write_sethi(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode);

static int _sparc_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	uint32_t opcode = instruction->opcode;

	if((opcode & 0xc0000000) == 0xc0000000)
	{
		if(_write_loadstore(plugin, instruction, call, &opcode) != 0)
			return -1;
	}
	else if((opcode & 0xc1c00000) == 0x01000000)
	{
		if(_write_sethi(plugin, instruction, call, &opcode) != 0)
			return -1;
	}
	else if((opcode & 0xc0000000) == 0x80000000)
	{
		if(_write_integer(plugin, instruction, call, &opcode) != 0)
			return -1;
	}
	else if((opcode & 0xc1c00000) == 0x00800000)
	{
		if(_write_branch(plugin, instruction, call, &opcode) != 0)
			return -1;
	}
	else
		return -1;
	opcode = _htob32(opcode);
	if(fwrite(&opcode, sizeof(opcode), 1, plugin->helper->fp) != 1)
		return -1;
	return 0;
}

static int _write_branch(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode)
{
	uint32_t disp;

	if(AO_GET_TYPE(instruction->op1) != AOT_IMMEDIATE)
		return -1;
	disp = call->operands[0].value.immediate.value;
	if(call->operands[0].value.immediate.negative)
		disp = -disp;
	disp &= (0x003fffff);
	*opcode |= disp;
	return 0;
}

static int _write_integer(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
	char const * name;
	ArchRegister * ar;

	/* rs1 */
	if(AO_GET_TYPE(instruction->op1) != AOT_REGISTER)
		return -1;
	name = call->operands[0].value._register.name;
	if((ar = helper->get_register_by_name_size(helper->arch,
					name, 32)) == NULL)
		return -1;
	rs1 = (ar->id << 14);
	/* rs2 */
	if(AO_GET_TYPE(instruction->op2) == AOT_REGISTER)
	{
		name = call->operands[1].value._register.name;
		if((ar = helper->get_register_by_name_size(helper->arch,
						name, 32)) == NULL)
			return -1;
		rs2 = ar->id;
	}
	else if(AO_GET_TYPE(instruction->op2) == AOT_IMMEDIATE)
	{
		rs2 = call->operands[1].value.immediate.value;
		if(call->operands[1].value.immediate.negative)
			rs2 = -rs2;
		rs2 &= 0x00001fff;
		rs2 |= (1 << 13);
	}
	else
		return -1;
	/* rd */
	if(AO_GET_TYPE(instruction->op3) != AOT_REGISTER)
		return -1;
	name = call->operands[2].value._register.name;
	if((ar = helper->get_register_by_name_size(helper->arch,
					name, 32)) == NULL)
		return -1;
	rd = (ar->id << 25);
	*opcode |= (rd | rs1 | rs2);
	return 0;
}

static int _write_loadstore(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
	char const * name;
	ArchRegister * ar;

	if(instruction->opcode & (1 << 21)) /* store instruction */
	{
		/* rd */
		if(AO_GET_TYPE(instruction->op1) != AOT_REGISTER)
			return -1;
		name = call->operands[0].value._register.name;
		if((ar = helper->get_register_by_name_size(helper->arch,
						name, 32)) == NULL)
			return -1;
		rd = (ar->id << 25);
		/* [rs1 + rs2] */
		if(AO_GET_TYPE(instruction->op2) == AOT_DREGISTER2)
		{
			name = call->operands[1].value.dregister2.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							name, 32)) == NULL)
				return -1;
			rs1 = (ar->id << 14);
			name = call->operands[1].value.dregister2.name2;
			if((ar = helper->get_register_by_name_size(helper->arch,
							name, 32)) == NULL)
				return -1;
			rs2 = ar->id;
		}
		else if(AO_GET_TYPE(instruction->op2) == AOT_DREGISTER)
		{
			name = call->operands[1].value.dregister.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							name, 32)) == NULL)
				return -1;
			rs1 = (ar->id << 14);
			rs2 = 0; /* FIXME implement */
		}
		else
			return -1;
	}
	else
	{
		/* [rs1 + rs2] */
		if(AO_GET_TYPE(instruction->op1) == AOT_DREGISTER2)
		{
			name = call->operands[0].value.dregister2.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							name, 32)) == NULL)
				return -1;
			rs1 = (ar->id << 14);
			name = call->operands[0].value.dregister2.name2;
			if((ar = helper->get_register_by_name_size(helper->arch,
							name, 32)) == NULL)
				return -1;
			rs2 = ar->id;
		}
		else if(AO_GET_TYPE(instruction->op1) == AOT_DREGISTER)
		{
			name = call->operands[0].value.dregister.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							name, 32)) == NULL)
				return -1;
			rs1 = (ar->id << 14);
			rs2 = 0; /* FIXME implement */
		}
		else
			return -1;
		/* rd */
		if(AO_GET_TYPE(instruction->op2) != AOT_REGISTER)
			return -1;
		name = call->operands[1].value._register.name;
		if((ar = helper->get_register_by_name_size(helper->arch,
						name, 32)) == NULL)
			return -1;
		rd = (ar->id << 25);
	}
	*opcode |= (rd | rs1 | rs2);
	return 0;
}

static int _write_sethi(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call, uint32_t * opcode)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t rd;
	uint32_t value;
	char const * name;
	ArchRegister * ar;

	/* value */
	if(AO_GET_TYPE(instruction->op1) != AOT_IMMEDIATE)
		return -1;
	value = (call->operands[0].value.immediate.value >> 10);
	/* rd */
	if(AO_GET_TYPE(instruction->op2) != AOT_REGISTER)
		return -1;
	name = call->operands[1].value._register.name;
	if((ar = helper->get_register_by_name_size(helper->arch,
					name, 32)) == NULL)
		return -1;
	rd = (ar->id << 25);
	*opcode |= (rd | value);
	return 0;
}
