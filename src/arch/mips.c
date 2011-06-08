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


/* mips */
/* private */
/* variables */
static ArchDescription _mips_description = { "elf", ARCH_ENDIAN_BIG, 32, 32 };

#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _mips_registers[] =
{
#include "mips.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _mips_instructions[] =
{
#include "mips.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
/* plug-in */
#include "mips.h"


/* protected */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"mips",
	&_mips_description,
	_mips_registers,
	_mips_instructions,
	_mips_write,
	NULL
};
