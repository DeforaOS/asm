/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
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


/* constants */
#ifndef ARCH_ENDIAN
# define ARCH_ENDIAN	ASM_ARCH_ENDIAN_BOTH
#endif


/* arm */
/* private */
/* types */
/* register ids */
#define REG(name, size, id) REG_ ## name ## _id = id,
enum
{
#include "arm.reg"
	REG_id_count
};
#undef REG


/* variables */
static AsmArchDescription _arm_description =
{
	"elf", ARCH_ENDIAN, 32, 32, 32
};

#define REG(name, size, id) { "" # name, size, id },
static AsmArchRegister _arm_registers[] =
{
#include "arm.reg"
	{ NULL,		0, 0 }
};
#undef REG

static AsmArchInstruction _arm_instructions[] =
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
AsmArchPlugin arch_plugin =
{
	NULL,
	"arm",
	&_arm_description,
	_arm_registers,
	_arm_instructions,
	NULL,
	NULL,
	_arm_encode,
	_arm_decode
};
