/* $Id$ */
/* Copyright (c) 2011-2018 Pierre Pronchery <khorben@defora.org> */
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
#include <string.h>
#include "Asm.h"
#define ARCH_amd64


/* amd64 */
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
#include "amd64.reg"
#include "i386.reg"
#include "i686.reg"
	REG_id_count
};
#undef REG


/* variables */
static AsmArchDefinition const _amd64_definition =
{
#if defined(__APPLE__)
	"mach-o", ASM_ARCH_ENDIAN_LITTLE, 64, 8, 0
#elif defined(__WIN32__)
	"pe", ASM_ARCH_ENDIAN_LITTLE, 64, 8, 0
#else
	"elf", ASM_ARCH_ENDIAN_LITTLE, 64, 8, 0
#endif
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _amd64_registers[] =
{
#include "amd64.reg"
#include "i386.reg"
#include "i686.reg"
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _amd64_instructions[] =
{
#include "i386.ins"
#include "i486.ins"
#include "i586.ins"
#include "i686.ins"
#include "amd64.ins"
#include "common.ins"
#include "null.ins"
};

static AsmArchPrefix const _amd64_prefixes[] =
{
#include "i386.pre"
#include "amd64.pre"
#include "null.pre"
};


/* functions */
#include "i386.h"


/* public */
/* variables */
/* plug-in */
AsmArchPluginDefinition arch_plugin =
{
	"amd64",
	"AMD64",
	LICENSE_GNU_LGPL3_FLAGS,
	&_amd64_definition,
	_amd64_registers,
	_amd64_prefixes,
	_amd64_instructions,
	_i386_init,
	_i386_destroy,
	_i386_encode,
	_i386_decode
};
