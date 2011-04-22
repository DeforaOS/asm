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

	/* internal */
	/* file */
	char const * filename;
	FILE * fp;
};


/* prototypes */
/* callbacks */
static char const * _format_get_filename(Format * format);

static ssize_t _format_read(Format * format, void * buf, size_t size);
static off_t _format_seek(Format * format, off_t offset, int whence);

static ssize_t _format_write(Format * format, void const * buf, size_t size);


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
	format->helper.format = NULL;
	format->helper.read = NULL;
	format->helper.seek = NULL;
	format->plugin->helper = NULL;
	format->fp = NULL;
	format->filename = NULL;
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
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename,
			(void *)fp);
#endif
	format->filename = filename;
	format->fp = fp;
	format->helper.format = format;
	format->helper.get_filename = _format_get_filename;
	format->helper.read = _format_read;
	format->helper.seek = _format_seek;
	format->helper.write = _format_write;
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


/* private */
/* functions */
/* format_get_filename */
static char const * _format_get_filename(Format * format)
{
	return format->filename;
}


/* format_read */
static ssize_t _format_read(Format * format, void * buf, size_t size)
{
	if(fread(buf, size, 1, format->fp) == 1)
		return size;
	if(ferror(format->fp))
		return -error_set_code(1, "%s: %s", format->filename,
				strerror(errno));
	if(feof(format->fp))
		return -error_set_code(1, "%s: %s", format->filename,
				"End of file reached");
	return -error_set_code(1, "%s: %s", format->filename, "Read error");
}


/* format_seek */
static off_t _format_seek(Format * format, off_t offset, int whence)
{
	if(whence == SEEK_SET)
	{
		if(fseek(format->fp, offset, whence) == 0)
			return offset;
	}
	else if(whence == SEEK_CUR || whence == SEEK_END)
	{
		if(fseek(format->fp, offset, whence) == 0)
			return ftello(format->fp);
	}
	else
		return -error_set_code(1, "%s: %s", format->filename,
				"Invalid argument for seeking");
	return -error_set_code(1, "%s: %s", format->filename, strerror(errno));
}


/* format_write */
static ssize_t _format_write(Format * format, void const * buf, size_t size)
{
	if(fwrite(buf, size, 1, format->fp) == 1)
		return size;
	if(ferror(format->fp))
		return -error_set_code(1, "%s: %s", format->filename,
				strerror(errno));
	if(feof(format->fp))
		return -error_set_code(1, "%s: %s", format->filename,
				"End of file reached");
	return -error_set_code(1, "%s: %s", format->filename, "Write error");
}
