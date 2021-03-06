/* $Id$ */
/* Copyright (c) 2011-2018 Pierre Pronchery <khorben@defora.org> */
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


/* sparc */
/* private */
/* variables */
static AsmArchDefinition const _sparc_definition =
{
	"elf", ASM_ARCH_ENDIAN_BIG, 32, 32, 32
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _sparc_registers[] =
{
#include "sparc.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _sparc_instructions[] =
{
#include "sparc.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
/* plug-in */
#include "sparc.h"


/* protected */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	"sparc",
	"Sun SPARC (32-bits)",
	LICENSE_GNU_LGPL3_FLAGS,
	&_sparc_definition,
	_sparc_registers,
	NULL,
	_sparc_instructions,
	_sparc_init,
	_sparc_destroy,
	_sparc_encode,
	_sparc_decode
};
