/* $Id$ */
/* Copyright (c) 2011-2017 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel Asm */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <stdio.h>
#include <string.h>
#include <System.h>
#include "Asm.h"


/* Dalvik */
/* private */
/* types */
struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
};

typedef struct _DalvikDecode
{
	AsmArchPlugin * plugin;
	AsmArchInstructionCall * call;

	int u8;
} DalvikDecode;


/* constants */
/* register sizes */
#define REG(name, size, id, flags, description) \
	REG_dalvik_ ## name ## _size = size,
enum
{
#include "dalvik.reg"
	REG_dalvik_size_count
};
#undef REG

/* register ids */
#define REG(name, size, id, flags, description) REG_dalvik_ ## name ## _id = id,
enum
{
#include "dalvik.reg"
	REG_dalvik_id_count
};
#undef REG


/* variables */
/* plug-in */
static AsmArchDefinition const _dalvik_definition =
{
	"dex", ASM_ARCH_ENDIAN_LITTLE, 32, 16, 0
};


#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _dalvik_registers[] =
{
#include "dalvik.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _dalvik_instructions[] =
{
#include "dalvik.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
/* plug-in */
static AsmArchPlugin * _dalvik_init(AsmArchPluginHelper * helper);
static void _dalvik_destroy(AsmArchPlugin * plugin);
static int _dalvik_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);
static int _dalvik_decode(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call);


/* public */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	"dalvik",
	"Dalvik bytecode",
	LICENSE_GNU_LGPL3_FLAGS,
	&_dalvik_definition,
	_dalvik_registers,
	_dalvik_instructions,
	_dalvik_init,
	_dalvik_destroy,
	_dalvik_encode,
	_dalvik_decode
};


/* private */
/* functions */
/* dalvik_init */
static AsmArchPlugin * _dalvik_init(AsmArchPluginHelper * helper)
{
	AsmArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* dalvik_destroy */
static void _dalvik_destroy(AsmArchPlugin * plugin)
{
	object_delete(plugin);
}


/* dalvik_encode */
static int _dalvik_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
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
static int _dalvik_decode_immediate(DalvikDecode * dd, size_t i);
static int _dalvik_decode_operand(DalvikDecode * dd, size_t i);
static int _dalvik_decode_register(DalvikDecode * dd, size_t i);

static int _dalvik_decode(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call)
{
	DalvikDecode dd;
	AsmArchPluginHelper * helper = plugin->helper;
	uint8_t u8;
	uint16_t u16;
	AsmArchInstruction const * ai;
	size_t i;

	dd.plugin = plugin;
	dd.call = call;
	dd.u8 = -1;
	/* FIXME detect end of input */
	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 8, u8))
			== NULL)
	{
		u16 = u8;
		if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		{
			call->name = "db";
			call->operands[0].definition = AO_IMMEDIATE(0, 8, 0);
			call->operands[0].value.immediate.name = NULL;
			call->operands[0].value.immediate.value = u16;
			call->operands[0].value.immediate.negative = 0;
			call->operands_cnt = 1;
			return 0;
		}
		u16 = _htol16((u16 << 8) | u8);
		if((ai = helper->get_instruction_by_opcode(helper->arch, 16,
						u16)) == NULL)
		{
			call->name = "dw";
			call->operands[0].definition = AO_IMMEDIATE(0, 16, 0);
			call->operands[0].value.immediate.name = NULL;
			call->operands[0].value.immediate.value = u16;
			call->operands[0].value.immediate.negative = 0;
			call->operands_cnt = 1;
			return 0;
		}
	}
	call->name = ai->name;
	call->operands[0].definition = ai->op1;
	call->operands[1].definition = ai->op2;
	call->operands[2].definition = ai->op3;
	for(i = 0; i < 3 && AO_GET_TYPE(call->operands[i].definition)
			!= AOT_NONE; i++)
		if(_dalvik_decode_operand(&dd, i) != 0)
			return -1;
	call->operands_cnt = i;
	return 0;
}

static int _dalvik_decode_immediate(DalvikDecode * dd, size_t i)
{
	AsmArchPluginHelper * helper = dd->plugin->helper;
	AsmArchOperand * ao = &dd->call->operands[i];
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	AsmFunction * af;
	AsmString * as;

	switch(AO_GET_SIZE(ao->definition))
	{
		case 4:
			if(dd->u8 >= 0)
			{
				ao->value.immediate.value = dd->u8 & 0xf;
				dd->u8 = -1;
				break;
			}
			if(helper->read(helper->arch, &u8, sizeof(u8))
					!= sizeof(u8))
				return -1;
			ao->value.immediate.value = u8 >> 4;
			dd->u8 = u8;
			break;
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
			return -error_set_code(1, "%s", "Unsupported immediate"
					" operand");
	}
	switch(AO_GET_VALUE(ao->definition))
	{
		case AOI_REFERS_FUNCTION:
			af = helper->get_function_by_id(helper->arch,
					ao->value.immediate.value);
			if(af != NULL)
				ao->value.immediate.name = af->name;
			break;
		case AOI_REFERS_STRING:
			as = helper->get_string_by_id(helper->arch,
					ao->value.immediate.value);
			if(as != NULL)
				ao->value.immediate.name = as->name;
			break;
	}
	ao->value.immediate.negative = 0;
	return 0;
}

static int _dalvik_decode_operand(DalvikDecode * dd, size_t i)
{
	switch(AO_GET_TYPE(dd->call->operands[i].definition))
	{
		case AOT_IMMEDIATE:
			return _dalvik_decode_immediate(dd, i);
		case AOT_REGISTER:
			return _dalvik_decode_register(dd, i);
		default:
			return -error_set_code(1, "%s", "Unsupported operand"
					" type");
	}
	return 0;
}

static int _dalvik_decode_register(DalvikDecode * dd, size_t i)
{
	AsmArchPluginHelper * helper = dd->plugin->helper;
	AsmArchOperandDefinition aod = dd->call->operands[i].definition;
	uint32_t id;
	uint8_t u8;
	uint16_t u16;
	AsmArchRegister const * ar;

	if(AO_GET_FLAGS(aod) & AOF_IMPLICIT)
		id = AO_GET_VALUE(aod);
	else if(AO_GET_FLAGS(aod) & AOF_DALVIK_REGSIZE)
	{
		switch(AO_GET_VALUE(aod))
		{
			case 4:
				if(dd->u8 >= 0)
				{
					id = dd->u8 & 0xf;
					dd->u8 = -1;
					break;
				}
				if(helper->read(helper->arch, &u8, sizeof(u8))
						!= sizeof(u8))
					return -1;
				id = u8 >> 4;
				dd->u8 = u8;
				break;
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
			default:
				return -1;
		}
	}
	else
		return -error_set_code(1, "%s", "Unsupported register operand");
	if(id >= 256)
		/* FIXME give the real name instead */
		dd->call->operands[i].value._register.name = ">256";
	else if((ar = helper->get_register_by_id_size(helper->arch, id, 32))
			!= NULL)
		dd->call->operands[i].value._register.name = ar->name;
	else
		return -1;
	return 0;
}
