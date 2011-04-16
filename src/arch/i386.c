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
static int _write_immediate(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand);
static int _write_immediate8(ArchPlugin * plugin, uint8_t value);
static int _write_immediate16(ArchPlugin * plugin, uint16_t value);
static int _write_immediate32(ArchPlugin * plugin, uint32_t value);
static int _write_opcode(ArchPlugin * plugin, ArchInstruction * instruction);
static int _write_operand(ArchPlugin * plugin, ArchOperandDefinition definition,
		ArchOperand * operand);
static int _write_register(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand);

static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	size_t i;
	ArchOperandDefinition definition;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	if(_write_opcode(plugin, instruction) != 0)
		return -1;
	for(i = 0; i < call->operands_cnt; i++)
	{
		definition = (i == 0) ? instruction->op1 : ((i == 1)
				? instruction->op2 : instruction->op3);
		if(_write_operand(plugin, definition, &call->operands[i]) != 0)
			return -1;
	}
	return 0;
}

static int _write_immediate(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	size_t size;

	size = AO_GET_SIZE(definition) >> 3;
	switch(size)
	{
		case 0:
			return 0;
		case sizeof(uint8_t):
			return _write_immediate8(plugin,
					operand->value.immediate.value);
		case sizeof(uint16_t):
			return _write_immediate16(plugin,
					operand->value.immediate.value);
		case sizeof(uint32_t):
			return _write_immediate32(plugin,
					operand->value.immediate.value);
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

	memset(&operand, 0, sizeof(operand));
	operand.type = AOT_IMMEDIATE;
	operand.value.immediate.value = instruction->opcode;
	return _write_immediate(plugin, instruction->flags, &operand);
}

static int _write_operand(ArchPlugin * plugin, ArchOperandDefinition definition,
		ArchOperand * operand)
{
	switch(operand->type)
	{
		case AOT_IMMEDIATE:
			return _write_immediate(plugin, definition, operand);
		case AOT_REGISTER:
			return _write_register(plugin, definition, operand);
	}
	return 0;
}

static int _write_register(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition idefinition;
	ArchOperand ioperand;
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);

	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT)
		return 0;
	idefinition = AO_IMMEDIATE(0, 0, 8);
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.type = AOT_IMMEDIATE;
	ioperand.value.immediate.value = helper->get_register_by_name_size(
			helper->priv, name, size);
	return _write_immediate(plugin, idefinition, &ioperand);
}
