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
static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


/* public */
/* variables */
/* plug-in */
ArchPlugin arch_plugin =
{
	NULL,
	"i386",
	NULL,
	_i386_registers,
	_i386_instructions,
	_i386_write,
	NULL
};



/* functions */
static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	unsigned char * buf;
	uint32_t size;
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	/* opcode */
	size = (AO_GET_SIZE(instruction->flags) >> 3);
	switch(size)
	{
		case 0:
			break;
		case sizeof(u8):
			u8 = instruction->opcode;
			buf = &u8;
			break;
		case sizeof(u16):
			u16 = _htob16(instruction->opcode);
			buf = &u16;
			break;
		case sizeof(u32):
			u32 = _htob32(instruction->opcode);
			buf = &u32;
			break;
		default:
			return -1;
	}
	if(size > 0 && fwrite(buf, size, 1, plugin->helper->fp) != 1)
		return -1;
	/* FIXME implement the rest */
	return 0;
}
