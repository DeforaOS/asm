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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm/format.h"
#include "format.h"
#include "../config.h"


/* Format */
/* private */
/* types */
struct _Format
{
	char * arch;
	FormatPluginHelper helper;
	Plugin * handle;
	FormatPlugin * plugin;
};


/* public */
/* functions */
/* format_new */
Format * format_new(char const * format, char const * arch)
{
	Format * f;
	Plugin * handle;
	FormatPlugin * plugin;

	if(format == NULL)
	{
		error_set_code(1, "%s", strerror(EINVAL));
		return NULL;
	}
	if((handle = plugin_new(LIBDIR, PACKAGE, "format", format)) == NULL)
		return NULL;
	if((plugin = plugin_lookup(handle, "format_plugin")) == NULL
			|| (f = object_new(sizeof(*f))) == NULL)
	{
		plugin_delete(handle);
		return NULL;
	}
	f->arch = string_new(arch);
	memset(&f->helper, 0, sizeof(f->helper));
	f->plugin = plugin;
	f->handle = handle;
	if(f->arch == NULL)
	{
		format_delete(f);
		return NULL;
	}
	return f;
}


/* format_delete */
void format_delete(Format * format)
{
	plugin_delete(format->handle);
	string_delete(format->arch);
	object_delete(format);
}


/* accessors */
/* format_get_name */
char const * format_get_name(Format * format)
{
	return format->plugin->name;
}


/* useful */
/* format_exit */
int format_exit(Format * format)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(format->plugin->exit != NULL)
		ret = format->plugin->exit(format->plugin);
	format->plugin->helper = NULL;
	format->helper.fp = NULL;
	format->helper.filename = NULL;
	format->helper.priv = NULL;
	return ret;
}


/* format_function */
int format_function(Format * format, char const * function)
{
	if(format->plugin->function == NULL)
		return 0;
	return format->plugin->function(format->plugin, function);
}


/* format_init */
int format_init(Format * format, char const * filename, FILE * fp)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename, fp);
#endif
	format->helper.filename = filename;
	format->helper.fp = fp;
	format->helper.priv = NULL;
	format->plugin->helper = &format->helper;
	if(format->plugin->init != NULL)
		return format->plugin->init(format->plugin, format->arch);
	return 0;
}


/* format_section */
int format_section(Format * format, char const * section)
{
	if(format->plugin->section == NULL)
		return 0;
	return format->plugin->section(format->plugin, section);
}
