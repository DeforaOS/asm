/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel Asm */
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


/* mips */
/* private */
/* prototypes */
/* plug-in */
static int _mips_encode(AsmArchPlugin * plugin,
		AsmArchInstruction * instruction,
		AsmArchInstructionCall * call);


/* functions */
/* plug-in */
/* mips_encode */
static int _mips_encode(AsmArchPlugin * plugin,
		AsmArchInstruction * instruction,
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
