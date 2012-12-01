/* $Id$ */
/* Copyright (c) 2012 Pierre Pronchery <khorben@defora.org> */
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



#undef format_plugin
#define format_plugin format_plugin_dex
#include "../src/format/dex.c"

#undef format_plugin
#define format_plugin format_plugin_elf
#include "../src/format/elf.c"

#undef format_plugin
#define format_plugin format_plugin_flat
#include "../src/format/flat.c"

#undef format_plugin
#define format_plugin format_plugin_java
#include "../src/format/java.c"

#undef format_plugin
#define format_plugin format_plugin_pe
#include "../src/format/pe.c"

#include "../src/format.c"


/* AsmFormat */
/* private */
/* constants */
static const struct
{
	char const * name;
	AsmFormatPlugin * plugin;
} _formats[] =
{
	{ "dex",	&format_plugin_dex	},
	{ "elf",	&format_plugin_elf	},
	{ "flat",	&format_plugin_flat	},
	{ "java",	&format_plugin_java	},
	{ "pe",		&format_plugin_pe	}
};


/* public */
/* functions */
/* format_new */
AsmFormat * format_new(char const * format)
{
	AsmFormat * f;
	AsmFormatPlugin * plugin = NULL;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
	if(format == NULL)
	{
		error_set_code(1, "%s", strerror(EINVAL));
		return NULL;
	}
	for(i = 0; i < sizeof(_formats) / sizeof(*_formats); i++)
		if(strcmp(_formats[i].name, format) == 0)
		{
			plugin = _formats[i].plugin;
			break;
		}
	if(plugin == NULL || (f = object_new(sizeof(*f))) == NULL)
		return NULL;
	f->handle = NULL;
	f->plugin = plugin;
	memset(&f->helper, 0, sizeof(f->helper));
	f->helper.format = f;
	f->helper.decode = _format_helper_decode;
	f->helper.get_filename = _format_helper_get_filename;
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
