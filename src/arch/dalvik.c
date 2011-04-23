/* $Id$ */
/* Copyright (c) 2011 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel asm */
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



#include <System.h>
#include <stdio.h>
#include "Asm.h"


/* Dalvik */
/* private */
/* constants */
/* register sizes */
#define REG(name, size, id) REG_ ## name ## _size = size,
enum
{
#include "dalvik.reg"
	REG_size_count
};
#undef REG

/* register ids */
#define REG(name, size, id) REG_ ## name ## _id = id,
enum
{
#include "dalvik.reg"
	REG_id_count
};
#undef REG


/* variables */
/* plug-in */
static ArchDescription _dalvik_description =
{ "dex", ARCH_ENDIAN_LITTLE, 2, 0 };


#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _dalvik_registers[] =
{
#include "dalvik.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _dalvik_instructions[] =
{
#include "dalvik.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
/* plug-in */
static int _dalvik_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);
static int _dalvik_decode(ArchPlugin * plugin, ArchInstructionCall * call);


/* public */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"dalvik",
	&_dalvik_description,
	_dalvik_registers,
	_dalvik_instructions,
	_dalvik_write,
	_dalvik_decode
};


/* private */
/* functions */
/* dalvik_write */
static int _dalvik_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint8_t u8;
	uint16_t u16;
	void const * buf;
	ssize_t size;

	/* FIXME really implement */
	switch(AO_GET_SIZE(instruction->flags))
	{
		case 8:
			u8 = instruction->opcode;
			buf = &u8;
			size = sizeof(u8);
			break;
		case 16:
			u16 = _htol16(instruction->opcode);
			buf = &u16;
			size = sizeof(u16);
			break;
		default:
			/* FIXME should not happen */
			return -error_set_code(1, "%s: %s",
					helper->get_filename(helper->arch),
					"Invalid size for opcode");
	}
	if(helper->write(helper->arch, buf, size) != size)
		return -1;
	return 0;
}


/* dalvik_decode */
static int _decode_immediate(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);
static int _decode_operand(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);
static int _decode_register(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);

static int _dalvik_decode(ArchPlugin * plugin, ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint8_t u8;
	ArchInstruction * ai;
	size_t i;

	/* FIXME detect end of input */
	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	call->operands[0].type = AOT_NONE;
	call->operands[1].type = AOT_NONE;
	call->operands[2].type = AOT_NONE;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 8, u8))
			== NULL)
		/* FIXME check if it's a nop or return "dw" */
		return -1;
	call->name = ai->name;
	call->operands[0].type = ai->op1;
	call->operands[1].type = ai->op2;
	call->operands[2].type = ai->op3;
	for(i = 0; AO_GET_TYPE(call->operands[i].type) != 0; i++)
		if(_decode_operand(plugin, call, i) != 0)
			return -1;
	call->operands_cnt = i;
	return 0;
}

static int _decode_immediate(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperand * ao = &call->operands[i];
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

	switch(AO_GET_SIZE(call->operands[i].type))
	{
		case 8:
			if(helper->read(helper->arch, &u8, sizeof(u8))
					!= sizeof(u8))
				return -1;
			ao->value.immediate.value = u8;
			break;
		case 16:
			if(helper->read(helper->arch, &u16, sizeof(u16))
					!= sizeof(u16))
				return -1;
			ao->value.immediate.value = _htol16(u16);
			break;
		case 32:
			if(helper->read(helper->arch, &u32, sizeof(u32))
					!= sizeof(u32))
				return -1;
			ao->value.immediate.value = _htol32(u32);
			break;
		default:
			return -1;
	}
	ao->value.immediate.negative = 0;
	return 0;
}

static int _decode_operand(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	switch(AO_GET_TYPE(call->operands[i].type))
	{
		case AOT_IMMEDIATE:
			return _decode_immediate(plugin, call, i);
		case AOT_REGISTER:
			return _decode_register(plugin, call, i);
		default:
			return -1;
	}
	return 0;
}

static int _decode_register(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t id;
	uint8_t u8;
	uint16_t u16;
	ArchRegister * ar;

	if(AO_GET_FLAGS(call->operands[i].type) & AOF_IMPLICIT)
		id = AO_GET_VALUE(call->operands[i].type);
	else if(AO_GET_FLAGS(call->operands[i].type) & AOF_DALVIK_REGSIZE)
	{
		switch(AO_GET_VALUE(call->operands[i].type))
		{
			case 8:
				if(helper->read(helper->arch, &u8, sizeof(u8))
						!= sizeof(u8))
					return -1;
				id = u8;
				break;
			case 16:
				if(helper->read(helper->arch, &u16, sizeof(u16))
						!= sizeof(u16))
					return -1;
				id = _htol16(u16);
				break;
			case 4:
				/* FIXME implement */
			default:
				return -1;
		}
	}
	else
		return -1;
	if((ar = helper->get_register_by_id_size(helper->arch, id, 32)) == NULL)
		return -1;
	call->operands[i].value._register.name = ar->name;
	return 0;
}
