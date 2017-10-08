/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
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



#ifndef ASM_ARCH_YASEP_H
# define ASM_ARCH_YASEP_H

# include <System.h>


/* yasep */
/* private */
/* types */
struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
};


/* prototypes */
/* plug-in */
static AsmArchPlugin * _yasep_init(AsmArchPluginHelper * helper);
static void _yasep_destroy(AsmArchPlugin * plugin);
static int _yasep_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);
static int _yasep_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call);


/* functions */
/* plug-in */
/* yasep_init */
static AsmArchPlugin * _yasep_init(AsmArchPluginHelper * helper)
{
	AsmArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* yasep_destroy */
static void _yasep_destroy(AsmArchPlugin * plugin)
{
	object_delete(plugin);
}


/* yasep_encode */
static int _encode_16(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);
static int _encode_32(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);

static int _yasep_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call)
{
	return (instruction->opcode & 0x1)
		? _encode_32(plugin, instruction, call)
		: _encode_16(plugin, instruction, call);
}

static int _encode_16(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint16_t u16 = instruction->opcode;
	AsmArchRegister const * ar;
	size_t size;
	char const * name;

	if((instruction->opcode & 0x03) == 0x0) /* RR */
	{
		name = call->operands[0].value._register.name;
		size = AO_GET_SIZE(instruction->op1);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		u16 |= ar->id << 12;
		name = call->operands[1].value._register.name;
		size = AO_GET_SIZE(instruction->op2);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		u16 |= ar->id << 8;
	}
	else if((instruction->opcode & 0x03) == 0x2) /* IR */
	{
		u16 |= call->operands[0].value.immediate.value << 12;
		name = call->operands[1].value._register.name;
		size = AO_GET_SIZE(instruction->op2);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		u16 |= ar->id << 8;
	}
	u16 = _htol16(u16);
	if(helper->write(helper->arch, &u16, sizeof(u16)) != sizeof(u16))
		return -1;
	return 0;
}

static int _encode_32(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint32_t opcode = instruction->opcode;

	opcode = _htol32(opcode);
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}


/* yasep_decode */
static int _yasep_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint16_t u16;
	uint16_t opcode;
	AsmArchInstruction const * ai;
	AsmArchRegister const * ar;

	if(helper->read(helper->arch, &u16, sizeof(u16)) != sizeof(u16))
		return -1;
	u16 = _htol16(u16);
	opcode = u16 & 0x00ff;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 16, opcode))
			== NULL)
		return -1;
	call->name = ai->name;
	if((opcode & 0x3) == 0x0) /* RR */
	{
		call->operands[0].definition = ai->op1;
		if((ar = helper->get_register_by_id_size(helper->arch,
						((u16 & 0xf000) >> 12) & 0xf,
						AO_GET_SIZE(ai->op1))) == NULL)
			return -1;
		call->operands[0].value._register.name = ar->name;
		call->operands[1].definition = ai->op2;
		if((ar = helper->get_register_by_id_size(helper->arch,
						((u16 & 0x0f00) >> 8) & 0xf,
						AO_GET_SIZE(ai->op2))) == NULL)
			return -1;
		call->operands[1].value._register.name = ar->name;
		call->operands_cnt = 2;
	}
	else if((opcode & 0x03) == 0x2) /* IR */
	{
		call->operands[0].definition = ai->op1;
		call->operands[0].value.immediate.value = (u16 & 0xf000) >> 12;
		call->operands[1].definition = ai->op2;
		if((ar = helper->get_register_by_id_size(helper->arch,
						((u16 & 0x0f00) >> 8) & 0xf,
						AO_GET_SIZE(ai->op2))) == NULL)
			return -1;
		call->operands[1].value._register.name = ar->name;
		call->operands_cnt = 2;
	}
	return 0;
}

#endif /* ASM_ARCH_YASEP_H */
