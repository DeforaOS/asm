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
#include "Asm/arch.h"


/* sparc */
/* private */
/* variables */
static ArchDescription _sparc_description = { ARCH_ENDIAN_BIG, 4, 4 };

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
static int _sparc_filter(ArchPlugin * arch, ArchInstruction * instruction);


/* protected */
/* variables */
ArchPlugin arch_plugin =
{
	"sparc",
	"elf",
	&_sparc_description,
	_sparc_registers,
	_sparc_instructions,
	_sparc_filter
};


/* private */
/* functions */
/* sparc_filter */
static int _sparc_filter(ArchPlugin * arch, ArchInstruction * instruction)
{
	/* FIXME implement */
	return 0;
}
