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
#include <string.h>
#include "Asm.h"


/* amd64 */
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
#include "i686.reg"
#include "amd64.reg"
	REG_id_count
};
#undef REG


/* variables */
#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _amd64_registers[] =
{
#include "i386.reg"
#include "i686.reg"
#include "amd64.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _amd64_instructions[] =
{
#include "i386.ins"
#include "i486.ins"
#include "i686.ins"
#include "common.ins"
#include "null.ins"
};


/* functions */
#include "i386.h"


/* public */
/* variables */
/* plug-in */
ArchPlugin arch_plugin =
{
	NULL,
	"amd64",
	NULL,
	_amd64_registers,
	_amd64_instructions,
	_i386_write,
	_i386_decode
};
