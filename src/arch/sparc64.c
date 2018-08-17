/* $Id$ */
/* Copyright (c) 2011-2015 Pierre Pronchery <khorben@defora.org> */
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
static AsmArchDefinition const _sparc64_definition =
{
	"elf", ASM_ARCH_ENDIAN_BIG, 64, 32, 32
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _sparc64_registers[] =
{
#include "sparc.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _sparc64_instructions[] =
{
#include "sparc.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
#include "sparc.h"


/* protected */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	"sparc64",
	"Sun SPARC (64-bits)",
	LICENSE_GNU_LGPL3_FLAGS,
	&_sparc64_definition,
	_sparc64_registers,
	NULL,
	_sparc64_instructions,
	_sparc_init,
	_sparc_destroy,
	_sparc_encode,
	_sparc_decode
};
