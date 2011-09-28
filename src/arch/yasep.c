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
static int _yasep_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


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
	_yasep_write,
	NULL
};


/* private */
/* functions */
/* plug-in */
/* yasep_write */
static int _write_16(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);
static int _write_32(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);

static int _yasep_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	return (instruction->opcode & 0x1)
		? _write_32(plugin, instruction, call)
		: _write_16(plugin, instruction, call);
}

static int _write_16(ArchPlugin * plugin, ArchInstruction * instruction,
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

static int _write_32(ArchPlugin * plugin, ArchInstruction * instruction,
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
