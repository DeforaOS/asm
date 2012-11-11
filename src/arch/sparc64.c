/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
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



#include <stddef.h>
#include "Asm.h"


/* sparc64 */
/* private */
/* variables */
static AsmArchDescription _sparc64_description =
{
	"elf", ASM_ARCH_ENDIAN_BIG, 64, 32, 32
};

#define REG(name, size, id) { "" # name, size, id },
static AsmArchRegister _sparc64_registers[] =
{
#include "sparc.reg"
	{ NULL,		0, 0 }
};
#undef REG

static AsmArchInstruction _sparc64_instructions[] =
{
#include "sparc.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
#include "sparc.h"


/* protected */
/* variables */
AsmArchPlugin arch_plugin =
{
	NULL,
	"sparc64",
	&_sparc64_description,
	_sparc64_registers,
	_sparc64_instructions,
	NULL,
	NULL,
	_sparc_encode,
	_sparc_decode
};
