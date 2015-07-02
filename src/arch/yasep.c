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
#include <System.h>
#include "Asm.h"
#ifndef ARCH_yasep
# define ARCH_yasep		32
# define ARCH_yasep32
# define ARCH_NAME		"yasep"
# define ARCH_DESCRIPTION	"YASEP"
#endif


/* yasep */
/* private */
/* variables */
/* plug-in */
static AsmArchDefinition const _yasep_definition =
{
	"flat", ASM_ARCH_ENDIAN_LITTLE, 32, 16, 0
};

#define REG(name, size, id, description) { "" # name, size, id, description },
static AsmArchRegister const _yasep_registers[] =
{
#include "yasep.reg"
	{ NULL, 0, 0, NULL }
};
#undef REG

static AsmArchInstruction const _yasep_instructions[] =
{
#include "yasep.ins"
#include "common.ins"
#include "null.ins"
};


/* prototypes */
/* plug-in */
static AsmArchPlugin * _yasep_init(AsmArchPluginHelper * helper);
static void _yasep_destroy(AsmArchPlugin * plugin);
static int _yasep_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall * call);
static int _yasep_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call);


/* public */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	ARCH_NAME,
	ARCH_DESCRIPTION,
	&_yasep_definition,
	_yasep_registers,
	_yasep_instructions,
	_yasep_init,
	_yasep_destroy,
	_yasep_encode,
	_yasep_decode	
};


/* private */
/* functions */
/* plug-in */
#include "yasep.h"
