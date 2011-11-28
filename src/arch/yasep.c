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
#include "Asm.h"


/* yasep */
/* private */
/* variables */
/* plug-in */
#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _yasep_registers[] =
{
#include "yasep.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _yasep_instructions[] =
{
#include "yasep.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
/* plug-in */
static int _yasep_encode(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);
static int _yasep_decode(ArchPlugin * plugin, ArchInstructionCall * call);


/* protected */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"yasep",
	NULL,
	_yasep_registers,
	_yasep_instructions,
	NULL,
	NULL,
	_yasep_encode,
	_yasep_decode	
};


/* private */
/* functions */
/* plug-in */
/* yasep_encode */
static int _encode_16(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);
static int _encode_32(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);

static int _yasep_encode(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	return (instruction->opcode & 0x1)
		? _encode_32(plugin, instruction, call)
		: _encode_16(plugin, instruction, call);
}

static int _encode_16(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint16_t opcode = instruction->opcode;

	opcode = _htob16(opcode);
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}

static int _encode_32(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t opcode = instruction->opcode;

	opcode = _htob32(opcode);
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}


/* yasep_decode */
static int _yasep_decode(ArchPlugin * plugin, ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint16_t u16;
	uint16_t opcode;
	ArchInstruction * ai;

	if(helper->read(helper->arch, &u16, sizeof(u16)) != sizeof(u16))
		return -1;
	u16 = _htob16(u16);
	opcode = u16 & 0x00ff;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 16, opcode))
			== NULL)
		return -1;
	call->name = ai->name;
	return 0;
}
