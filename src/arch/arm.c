/* $Id$ */
/* Copyright (c) 2011-2015 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel asm */
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


/* constants */
#ifndef ARCH_ENDIAN
# define ARCH_ENDIAN	ASM_ARCH_ENDIAN_BOTH
#endif
#ifndef ARCH_DESCRIPTION
# define ARCH_DESCRIPTION	"ARM"
#endif


/* arm */
/* private */
/* types */
/* register ids */
#define REG(name, size, id, flags, description) REG_ ## name ## _id = id,
enum
{
#include "arm.reg"
	REG_id_count
};
#undef REG


/* variables */
static AsmArchDefinition const _arm_definition =
{
	"elf", ARCH_ENDIAN, 32, 32, 32
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _arm_registers[] =
{
#include "arm.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _arm_instructions[] =
{
#include "arm.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
/* plug-in */
#include "arm.h"


/* protected */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	"arm",
	ARCH_DESCRIPTION,
	LICENSE_GNU_LGPL3_FLAGS,
	&_arm_definition,
	_arm_registers,
	_arm_instructions,
	_arm_init,
	_arm_destroy,
	_arm_encode,
	_arm_decode
};
