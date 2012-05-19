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



#include <System.h>
#include <string.h>
#include "Asm.h"


/* Flat */
/* private */
/* prototypes */
/* plug-in */
static int _flat_decode(AsmFormatPlugin * format, int raw);
static int _flat_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);


/* public */
/* variables */
AsmFormatPlugin format_plugin =
{
	NULL,
	"flat",
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	_flat_decode,
	_flat_decode_section,
	NULL
};


/* private */
/* functions */
/* plug-in */
/* flat_decode */
static int _flat_decode(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;
	off_t offset;

	if((offset = helper->seek(helper->format, 0, SEEK_END)) >= 0)
		return helper->set_section(helper->format, 0, ".text", 0,
				offset, 0);
	return -1;
}


/* flat_decode_section */
static int _flat_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;

	if(section->id != 0)
		return -1;
	return helper->decode(helper->format, section->offset, section->size,
			section->base, calls, calls_cnt);
}
