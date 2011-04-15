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
static int _sparc_write(ArchPlugin * arch, ArchInstruction * instruction,
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
static int _sparc_write(ArchPlugin * arch, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	uint32_t buf;

	buf = instruction->opcode;
	/* FIXME implement the rest */
	buf = _htob32(buf);
	if(fwrite(&buf, sizeof(buf), 1, arch->helper->fp) != 1)
		return -1;
	return 0;
}
