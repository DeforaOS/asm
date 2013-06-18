/* $Id$ */
/* Copyright (c) 2011-2013 Pierre Pronchery <khorben@defora.org> */
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



#ifndef ASM_ARCH_MIPS_H
# define ASM_ARCH_MIPS_H

#include <System.h>


/* mips */
/* private */
/* types */
struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
};


/* prototypes */
/* plug-in */
static AsmArchPlugin * _mips_init(AsmArchPluginHelper * helper);
static void _mips_destroy(AsmArchPlugin * plugin);
static int _mips_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall * call);


/* functions */
/* plug-in */
/* mips_init */
static AsmArchPlugin * _mips_init(AsmArchPluginHelper * helper)
{
	AsmArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* mips_destroy */
static void _mips_destroy(AsmArchPlugin * plugin)
{
	object_delete(plugin);
}


/* mips_encode */
static int _mips_encode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint32_t opcode = instruction->opcode;

	/* FIXME really implement */
	opcode = _htob32(opcode);
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}

#endif /* !ASM_ARCH_MIPS_H */
