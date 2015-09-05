/* $Id$ */
/* Copyright (c) 2012-2015 Pierre Pronchery <khorben@defora.org> */
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



#include "../src/format.c"


/* AsmFormat */
/* private */
/* constants */
extern AsmFormatPluginDefinition format_plugin_dex;
extern AsmFormatPluginDefinition format_plugin_elf;
extern AsmFormatPluginDefinition format_plugin_flat;
extern AsmFormatPluginDefinition format_plugin_java;
extern AsmFormatPluginDefinition format_plugin_mbr;
extern AsmFormatPluginDefinition format_plugin_pe;

static struct
{
	char const * name;
	AsmFormatPluginDefinition * definition;
} _formats[] =
{
	{ "dex",	NULL	},
	{ "elf",	NULL	},
	{ "flat",	NULL	},
	{ "java",	NULL	},
	{ "mbr",	NULL	},
	{ "pe",		NULL	}
};


/* prototypes */
static void _format_init(void);


/* public */
/* functions */
/* format_new */
AsmFormat * format_new(char const * format)
{
	AsmFormat * f;
	AsmFormatPluginDefinition * definition = NULL;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
	_format_init();
	if(format == NULL)
	{
		error_set_code(1, "%s", strerror(EINVAL));
		return NULL;
	}
	for(i = 0; i < sizeof(_formats) / sizeof(*_formats); i++)
		if(strcmp(_formats[i].name, format) == 0)
		{
			definition = _formats[i].definition;
			break;
		}
	if(definition == NULL)
	{
		error_set_code(1, "%s: %s", format, "Unsupported format");
		return NULL;
	}
	if((f = object_new(sizeof(*f))) == NULL)
		return NULL;
	f->handle = NULL;
	f->definition = definition;
	f->plugin = NULL;
	memset(&f->helper, 0, sizeof(f->helper));
	f->helper.format = f;
	f->helper.decode = _format_helper_decode;
	f->helper.get_filename = _format_helper_get_filename;
	f->helper.get_function_by_id = _format_helper_get_function_by_id;
	f->helper.get_functions = _format_helper_get_functions;
	f->helper.get_section_by_id = _format_helper_get_section_by_id;
	f->helper.get_string_by_id = _format_helper_get_string_by_id;
	f->helper.set_function = _format_helper_set_function;
	f->helper.set_section = _format_helper_set_section;
	f->helper.set_string = _format_helper_set_string;
	f->helper.read = _format_helper_read;
	f->helper.seek = _format_helper_seek;
	f->helper.write = _format_helper_write;
	return f;
}


/* format_new_match */
AsmFormat * format_new_match(char const * filename, FILE * fp)
{
	AsmFormat * format;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename, fp);
#endif
	_format_init();
	if(filename == NULL || fp == NULL)
	{
		error_set_code(1, "%s", strerror(EINVAL));
		return NULL;
	}
	for(i = 0; i < sizeof(_formats) / sizeof(*_formats); i++)
	{
		if((format = format_new(_formats[i].name)) == NULL)
			continue;
		if(format_init(format, NULL, filename, fp) == 0
				&& format_match(format) > 0)
			return format;
		format_delete(format);
	}
	return NULL;
}


/* private */
/* functions */
static void _format_init(void)
{
	if(_formats[0].definition == NULL)
	{
		_formats[0].definition = &format_plugin_dex;
		_formats[1].definition = &format_plugin_elf;
		_formats[2].definition = &format_plugin_flat;
		_formats[3].definition = &format_plugin_java;
		_formats[4].definition = &format_plugin_mbr;
		_formats[5].definition = &format_plugin_pe;
	}
}
