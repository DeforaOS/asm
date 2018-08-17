/* $Id$ */
/* Copyright (c) 2011-2017 Pierre Pronchery <khorben@defora.org> */
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
#include <string.h>
#include "Asm.h"


/* i386 */
/* private */
/* types */
/* register sizes */
#define REG(name, size, id, flags, description) REG_ ## name ## _size = size,
enum
{
#include "i386.reg"
	REG_size_count
};
#undef REG

/* register ids */
#define REG(name, size, id, flags, description) REG_ ## name ## _id = id,
enum
{
#include "i386.reg"
	REG_id_count
};
#undef REG


/* variables */
static AsmArchDefinition _i386_definition =
{
#include "i386.def"
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _i386_registers[] =
{
#include "i386.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _i386_instructions[] =
{
#include "i386.ins"
#include "common.ins"
#include "null.ins"
};

static AsmArchPrefix const _i386_prefixes[] =
{
#include "i386.pre"
#include "null.pre"
};


/* functions */
#include "i386.h"


/* public */
/* variables */
/* plug-in */
AsmArchPluginDefinition arch_plugin =
{
	"i386",
	"Intel 80386",
	LICENSE_GNU_LGPL3_FLAGS,
	&_i386_definition,
	_i386_registers,
	_i386_prefixes,
	_i386_instructions,
	_i386_init,
	_i386_destroy,
	_i386_encode,
	_i386_decode
};
