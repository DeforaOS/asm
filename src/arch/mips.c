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

#ifndef ARCH_ENDIAN
# define ARCH_ENDIAN		ASM_ARCH_ENDIAN_BOTH
#endif
#ifndef ARCH_DESCRIPTION
# define ARCH_DESCRIPTION	"MIPS"
#endif


/* mips */
/* private */
/* variables */
static AsmArchDefinition const _mips_definition =
{
	"elf", ARCH_ENDIAN, 32, 32, 32
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _mips_registers[] =
{
#include "mips.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _mips_instructions[] =
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
AsmArchPluginDefinition arch_plugin =
{
	"mips",
	ARCH_DESCRIPTION,
	LICENSE_GNU_LGPL3_FLAGS,
	&_mips_definition,
	_mips_registers,
	_mips_instructions,
	_mips_init,
	_mips_destroy,
	_mips_encode,
	NULL
};
