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


/* Dalvik */
/* private */
/* constants */
/* register sizes */
#define REG(name, size, id) REG_ ## name ## _size = size,
enum
{
#include "dalvik.reg"
	REG_size_count
};
#undef REG

/* register ids */
#define REG(name, size, id) REG_ ## name ## _id = id,
enum
{
#include "dalvik.reg"
	REG_id_count
};
#undef REG


/* variables */
/* plug-in */
static ArchDescription _dalvik_description =
{ "dex", ARCH_ENDIAN_LITTLE, 2, 0 };


#define REG(name, size, id) { "" # name, size, id },
static ArchRegister _dalvik_registers[] =
{
#include "dalvik.reg"
	{ NULL,		0, 0 }
};
#undef REG

static ArchInstruction _dalvik_instructions[] =
{
#include "dalvik.ins"
#include "common.ins"
#include "null.ins"
};


/* public */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"dalvik",
	&_dalvik_description,
	_dalvik_registers,
	_dalvik_instructions,
	NULL,
	NULL
};
