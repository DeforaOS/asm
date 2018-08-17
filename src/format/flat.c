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



#include <System.h>
#include <string.h>
#include "Asm.h"


/* Flat */
/* private */
/* types */
struct _AsmFormatPlugin
{
	AsmFormatPluginHelper * helper;
};


/* prototypes */
/* plug-in */
static AsmFormatPlugin * _flat_init(AsmFormatPluginHelper * helper,
		char const * arch);
static int _flat_destroy(AsmFormatPlugin * format);
static char const * _flat_guess(AsmFormatPlugin * format, char const * hint);
static int _flat_section(AsmFormatPlugin * format, char const * section);
static int _flat_decode(AsmFormatPlugin * format, int raw);
static int _flat_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);


/* public */
/* variables */
/* plug-in */
AsmFormatPluginDefinition format_plugin =
{
	"flat",
	"Flat file",
	LICENSE_GNU_LGPL3_FLAGS,
	NULL,
	0,
	_flat_init,
	_flat_destroy,
	_flat_guess,
	NULL,
	NULL,
	_flat_section,
	NULL,
	_flat_decode,
	_flat_decode_section
};


/* private */
/* functions */
/* plug-in */
/* flat_init */
static AsmFormatPlugin * _flat_init(AsmFormatPluginHelper * helper,
		char const * arch)
{
	AsmFormatPlugin * flat;
	(void) arch;

	if((flat = object_new(sizeof(*flat))) == NULL)
		return NULL;
	flat->helper = helper;
	return flat;
}


/* flat_destroy */
static int _flat_destroy(AsmFormatPlugin * format)
{
	object_delete(format);
	return 0;
}


/* flat_guess */
static char const * _flat_guess(AsmFormatPlugin * format, char const * hint)
{
	(void) format;

	return hint;
}


/* flat_section */
static int _flat_section(AsmFormatPlugin * format, char const * section)
{
	(void) format;
	(void) section;

	/* ignore sections */
	return 0;
}


/* flat_decode */
static int _flat_decode(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;
	off_t offset;
	(void) raw;

	if((offset = helper->seek(helper->format, 0, SEEK_END)) >= 0
			&& helper->set_section(helper->format, 0, 0, ".text", 0,
				offset, 0) != NULL)
		return 0;
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
