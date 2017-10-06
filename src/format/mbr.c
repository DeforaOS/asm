/* $Id$ */
/* Copyright (c) 2015-2017 Pierre Pronchery <khorben@defora.org> */
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


/* MBR */
/* private */
/* types */
struct _AsmFormatPlugin
{
	AsmFormatPluginHelper * helper;
};


/* constants */
static const AsmSectionId _mbr_section_id_text = 0;
static const AsmSectionId _mbr_section_id_data = 1;
static const AsmSectionId _mbr_section_id_signature = 2;
static const uint8_t _mbr_signature[2] = { 0x55, 0xaa };
static const size_t _mbr_size_text = 446;
static const size_t _mbr_size_data = 64;
static const uint8_t _mbr_zeros[512];


/* prototypes */
/* plug-in */
static AsmFormatPlugin * _mbr_init(AsmFormatPluginHelper * helper,
		char const * arch);
static int _mbr_destroy(AsmFormatPlugin * format);
static int _mbr_decode(AsmFormatPlugin * format, int raw);
static int _mbr_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);
static char const * _mbr_detect(AsmFormatPlugin * format);


/* public */
/* variables */
/* plug-in */
AsmFormatPluginDefinition format_plugin =
{
	"mbr",
	"MBR",
	LICENSE_GNU_LGPL3_FLAGS,
	NULL,
	0,
	_mbr_init,
	_mbr_destroy,
	NULL,
	NULL,
	_mbr_detect,
	_mbr_decode,
	_mbr_decode_section
};


/* private */
/* functions */
/* plug-in */
/* mbr_init */
static AsmFormatPlugin * _mbr_init(AsmFormatPluginHelper * helper,
		char const * arch)
{
	AsmFormatPlugin * mbr;

	if((mbr = object_new(sizeof(*mbr))) == NULL)
		return NULL;
	mbr->helper = helper;
	return mbr;
}


/* mbr_destroy */
static int _mbr_destroy(AsmFormatPlugin * format)
{
	int ret = 0;
	AsmFormatPluginHelper * helper = format->helper;
	long offset;
	ssize_t size;

	/* FIXME support writing to the data section too */
	if((offset = helper->seek(helper->format, 0, SEEK_CUR)) > 446)
		ret = -1;
	else
	{
		size = sizeof(_mbr_zeros) - sizeof(_mbr_signature) - offset;
		if(helper->write(helper->format, _mbr_zeros, size) != size
				|| helper->write(helper->format,
					_mbr_signature,
					sizeof(_mbr_signature))
				!= sizeof(_mbr_signature))
			ret = -1;
	}
	object_delete(format);
	return ret;
}


/* mbr_decode */
static int _mbr_decode(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;

	if(helper->seek(helper->format, 0, SEEK_END) >= 512
			&& helper->set_section(helper->format,
				_mbr_section_id_text, 0, ".text", 0,
				_mbr_size_text, 0) != NULL
			&& helper->set_section(helper->format,
				_mbr_section_id_data, 0, ".data",
				_mbr_size_text, _mbr_size_data, 0) != NULL
			&& helper->set_section(helper->format,
				_mbr_section_id_signature, 0, ".signature",
				_mbr_size_text + _mbr_size_data,
				sizeof(_mbr_signature), 0) != NULL)
		return 0;
	return -1;
}


/* mbr_decode_section */
static int _mbr_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;

	if(section->id == 0)
		return helper->decode(helper->format, section->offset,
				section->size, section->base,
				calls, calls_cnt);
	if(section->id == 1 || section->id == 2)
		/* FIXME decode as data */
		return 0;
	return -1;
}


/* mbr_detect */
static char const * _mbr_detect(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	uint8_t buf[512];

	if(helper->seek(helper->format, 0, SEEK_SET) != 0
			|| helper->read(helper->format, buf, sizeof(buf))
			!= sizeof(buf))
	{
		error_set_code(1, "%s: %s 0x%x", format_plugin.name,
				"Could not read the bootloader image");
		return NULL;
	}
	if(buf[510] != _mbr_signature[0] || buf[511] != _mbr_signature[1])
	{
		error_set_code(1, "%s: %s 0x%x", format_plugin.name,
				"Could not find the MBR signature");
		return NULL;
	}
	return "i386_real";
}
