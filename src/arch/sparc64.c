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


/* sparc64 */
/* private */
/* variables */
static ArchDescription _sparc64_description =
{ "elf", ARCH_ENDIAN_BIG, 32, 32 };

#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _sparc64_registers[] =
{
#include "sparc.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _sparc64_instructions[] =
{
#include "sparc.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
#include "sparc.h"


/* protected */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"sparc64",
	&_sparc64_description,
	_sparc64_registers,
	_sparc64_instructions,
	_sparc_write,
	_sparc_decode
};
