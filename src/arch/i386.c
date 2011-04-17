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



#include <stddef.h>
#include <string.h>
#include "Asm.h"


/* i386 */
/* private */
/* types */
/* register sizes */
#define REG(name, size, id) REG_ ## name ## _size = size,
enum
{
#include "i386.reg"
	REG_size_count
};
#undef REG

/* register ids */
#define REG(name, size, id) REG_ ## name ## _id = id,
enum
{
#include "i386.reg"
	REG_id_count
};
#undef REG


/* variables */
#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _i386_registers[] =
{
#include "i386.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _i386_instructions[] =
{
#include "i386.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


/* public */
/* variables */
/* plug-in */
ArchPlugin arch_plugin =
{
	NULL,
	"i386",
	NULL,
	_i386_registers,
	_i386_instructions,
	_i386_write,
	NULL
};



/* functions */
static int _write_dregister(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands);
static int _write_immediate(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand);
static int _write_immediate8(ArchPlugin * plugin, uint8_t value);
static int _write_immediate16(ArchPlugin * plugin, uint16_t value);
static int _write_immediate32(ArchPlugin * plugin, uint32_t value);
static int _write_opcode(ArchPlugin * plugin, ArchInstruction * instruction);
static int _write_operand(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands);
static int _write_register(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands);

static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	uint32_t i;
	ArchOperandDefinition definitions[3];

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	if(_write_opcode(plugin, instruction) != 0)
		return -1;
	definitions[0] = instruction->op1;
	definitions[1] = instruction->op2;
	definitions[2] = instruction->op3;
	for(i = 0; i < call->operands_cnt; i++)
		if(_write_operand(plugin, &i, definitions, call->operands) != 0)
			return -1;
	return 0;
}

static int _write_dregister(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition definition = definitions[*i];
	ArchOperand * operand = &operands[*i];
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);
	ArchRegister * ar;
	ArchOperandDefinition idefinition;
	ArchOperand ioperand;

	if((ar = helper->get_register_by_name_size(helper->arch, name, size))
			== NULL)
		return -1;
	/* write register */
	idefinition = AO_IMMEDIATE(0, 0, 8);
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.type = AOT_IMMEDIATE;
	ioperand.value.immediate.value = ar->id;
	if(AO_GET_FLAGS(definition) & AOF_I386_MODRM
			&& AO_GET_VALUE(definition) == 8) /* mod r/m, /r */
	{
		(*i)++; /* skip next operand */
		/* FIXME it could as well be an inverted /r */
		name = operands[*i].value._register.name;
		size = AO_GET_SIZE(definitions[*i]);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		ioperand.value.immediate.value |= (ar->id << 3);
	}
	else if(AO_GET_FLAGS(definition) & AOF_I386_MODRM) /* mod r/m, /[0-7] */
		ioperand.value.immediate.value |= (AO_GET_VALUE(definition)
				<< 3);
	if(operand->value.dregister.offset == 0)
		/* there is no offset */
		return _write_immediate(plugin, idefinition, &ioperand);
	/* declare offset */
	switch(AO_GET_OFFSET(definition) >> 3)
	{
		case sizeof(uint8_t):
			ioperand.value.immediate.value |= 0x40;
			break;
		case W >> 3:
			ioperand.value.immediate.value |= 0x80;
			break;
		default:
			return -1; /* FIXME report error */
	}
	if(_write_immediate(plugin, idefinition, &ioperand) != 0)
		return -1;
	/* write offset */
	idefinition = AO_IMMEDIATE(0, 0, AO_GET_OFFSET(definition));
	ioperand.value.immediate.value = operand->value.dregister.offset;
	return _write_immediate(plugin, idefinition, &ioperand);
}

static int _write_immediate(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	uint64_t value = operand->value.immediate.value;

	if((AO_GET_FLAGS(definition) & AOF_SIGNED)
			&& operand->value.immediate.negative != 0)
		value = -value;
	switch(AO_GET_SIZE(definition) >> 3)
	{
		case 0:
			return 0;
		case sizeof(uint8_t):
			return _write_immediate8(plugin, value);
		case sizeof(uint16_t):
			return _write_immediate16(plugin, value);
		case sizeof(uint32_t):
			return _write_immediate32(plugin, value);
		default:
			return -1;
	}
}

static int _write_immediate8(ArchPlugin * plugin, uint8_t value)
{
	if(fwrite(&value, sizeof(value), 1, plugin->helper->fp) != 1)
		return -1;
	return 0;
}

static int _write_immediate16(ArchPlugin * plugin, uint16_t value)
{
	value = _htol16(value);
	if(fwrite(&value, sizeof(value), 1, plugin->helper->fp) != 1)
		return -1;
	return 0;
}

static int _write_immediate32(ArchPlugin * plugin, uint32_t value)
{
	value = _htol32(value);
	if(fwrite(&value, sizeof(value), 1, plugin->helper->fp) != 1)
		return -1;
	return 0;
}

static int _write_opcode(ArchPlugin * plugin, ArchInstruction * instruction)
{
	ArchOperand operand;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() size=%u opcode=0x%x\n", __func__,
			AO_GET_SIZE(instruction->flags), instruction->opcode);
#endif
	memset(&operand, 0, sizeof(operand));
	operand.type = AOT_IMMEDIATE;
	switch(AO_GET_SIZE(instruction->flags) >> 3)
	{
		case sizeof(uint8_t):
			operand.value.immediate.value = instruction->opcode;
			break;
		case sizeof(uint16_t):
			operand.value.immediate.value = _htob16(
					instruction->opcode);
			break;
		case sizeof(uint32_t):
			operand.value.immediate.value = _htob32(
					instruction->opcode);
			break;
		default:
			return -1; /* FIXME report error */
	}
	return _write_immediate(plugin, instruction->flags, &operand);
}

static int _write_operand(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands)
{
	switch(operands[*i].type)
	{
		case AOT_DREGISTER:
			return _write_dregister(plugin, i, definitions,
					operands);
		case AOT_IMMEDIATE:
			return _write_immediate(plugin, definitions[*i],
					&operands[*i]);
		case AOT_REGISTER:
			return _write_register(plugin, i, definitions,
					operands);
	}
	return 0;
}

static int _write_register(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition definition = definitions[*i];
	ArchOperand * operand = &operands[*i];
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);
	ArchRegister * ar;
	ArchOperandDefinition idefinition;
	ArchOperand ioperand;

	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT)
		return 0;
	if((ar = helper->get_register_by_name_size(helper->arch, name, size))
			== NULL)
		return -1;
	/* write register */
	idefinition = AO_IMMEDIATE(0, 0, 8);
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.type = AOT_IMMEDIATE;
	ioperand.value.immediate.value = ar->id;
	if(AO_GET_FLAGS(definition) & AOF_I386_MODRM
			&& AO_GET_VALUE(definition) == 8) /* mod r/m, /r */
	{
		(*i)++; /* skip next operand */
		/* FIXME it could as well be an inverted /r */
		name = operands[*i].value._register.name;
		size = AO_GET_SIZE(definitions[*i]);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		ioperand.value.immediate.value |= 0xc0 | (ar->id << 3);
	}
	else if(AO_GET_FLAGS(definition) & AOF_I386_MODRM) /* mod r/m, /[0-7] */
		ioperand.value.immediate.value = 0xc0 | ar->id
			| (AO_GET_VALUE(definition) << 3);
	else
		ioperand.value.immediate.value = ar->id;
	return _write_immediate(plugin, idefinition, &ioperand);
}
