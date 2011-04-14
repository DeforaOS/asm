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
#ifdef DEBUG
# include <stdio.h>
#endif
#include "Asm/arch.h"


/* i386 */
/* private */
/* types */
/* register sizes */
#define REG(name, size, id) REG_ ## name ## _size = size,
enum
{
#include "i386.reg"
	REG_size_count
};
#undef REG

/* register ids */
#define REG(name, size, id) REG_ ## name ## _id = id,
enum
{
#include "i386.reg"
	REG_id_count
};
#undef REG


/* variables */
#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _i386_registers[] =
{
#include "i386.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _i386_instructions[] =
{
#include "i386.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
static int _i386_filter(ArchPlugin * plugin, ArchInstruction * instruction,
		unsigned char * buf, size_t size);


/* public */
/* variables */
/* plug-in */
ArchPlugin arch_plugin =
{
	"i386",
	"elf",
	NULL,
	_i386_registers,
	_i386_instructions,
	_i386_filter
};



/* functions */
static int _i386_filter(ArchPlugin * plugin, ArchInstruction * instruction,
		unsigned char * buf, size_t size)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() 0x%x\n", __func__, buf[0]);
#endif
	/* the filter function is only set for mod r/m bytes at the moment */
	buf[0] |= 0xc0;
	return 0;
}
