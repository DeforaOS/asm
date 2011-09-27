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
	FormatPluginHelper helper;
	Plugin * handle;
	FormatPlugin * plugin;

	/* internal */
	/* file */
	char const * filename;
	FILE * fp;

	/* deassembly */
	Code * code;
};


/* prototypes */
/* helpers */
static char const * _format_helper_get_filename(Format * format);
static AsmString * _format_helper_get_string_by_id(Format * format, AsmId id);
static int _format_helper_set_function(Format * format, int id,
		char const * name, off_t offset, ssize_t size);
static int _format_helper_set_string(Format * format, int id, char const * name,
		off_t offset, ssize_t size);

static int _format_helper_decode(Format * format, char const * section,
		off_t offset, size_t size, off_t base);
static ssize_t _format_helper_read(Format * format, void * buf, size_t size);
static off_t _format_helper_seek(Format * format, off_t offset, int whence);
static ssize_t _format_helper_write(Format * format, void const * buf,
		size_t size);


/* public */
/* functions */
/* format_new */
Format * format_new(char const * format)
{
	Format * f;
	Plugin * handle;
	FormatPlugin * plugin;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
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
	f->handle = handle;
	f->plugin = plugin;
	memset(&f->helper, 0, sizeof(f->helper));
	f->helper.format = f;
	f->helper.decode = _format_helper_decode;
	f->helper.get_filename = _format_helper_get_filename;
	f->helper.get_string_by_id = _format_helper_get_string_by_id;
	f->helper.set_function = _format_helper_set_function;
	f->helper.set_string = _format_helper_set_string;
	f->helper.read = _format_helper_read;
	f->helper.seek = _format_helper_seek;
	f->helper.write = _format_helper_write;
	return f;
}


/* format_delete */
void format_delete(Format * format)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	format_exit(format);
	plugin_delete(format->handle);
	object_delete(format);
}


/* accessors */
/* format_get_arch */
char const * format_get_arch(Format * format)
{
	if(format->plugin->detect == NULL)
		return NULL;
	return format->plugin->detect(format->plugin);
}


/* format_get_name */
char const * format_get_name(Format * format)
{
	return format->plugin->name;
}


/* useful */
/* format_decode */
int format_decode(Format * format, Code * code, int raw)
{
	int ret;

	if(format->plugin->decode == NULL)
		return error_set_code(1, "%s: %s", format_get_name(format),
				"Disassembly is not supported");
	format->code = code;
	ret = format->plugin->decode(format->plugin, raw);
	format->code = NULL;
	return ret;
}


/* format_detect_arch */
char const * format_detect_arch(Format * format)
{
	if(format->plugin->detect == NULL)
	{
		error_set_code(1, "%s: %s", format->plugin->name,
				"Unable to detect the architecture");
		return NULL;
	}
	return format->plugin->detect(format->plugin);
}


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
int format_init(Format * format, char const * arch, char const * filename,
		FILE * fp)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename,
			(void *)fp);
#endif
	if(format->plugin->helper != NULL)
		format_exit(format);
	format->filename = filename;
	format->fp = fp;
	format->plugin->helper = &format->helper;
	if(format->plugin->init != NULL)
		return format->plugin->init(format->plugin, arch);
	return 0;
}


/* format_match */
int format_match(Format * format)
{
	int ret = 0;
	char const * s = format->plugin->signature;
	ssize_t s_len = format->plugin->signature_len;
	char * buf = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(s_len > 0)
		if((buf = malloc(s_len)) == NULL)
			ret = -error_set_code(1, "%s", strerror(errno));
	if(buf != NULL)
	{
		if(_format_helper_seek(format, 0, SEEK_SET) != 0)
			ret = -1;
		else if(_format_helper_read(format, buf, s_len) != s_len)
			ret = -1;
		else if(memcmp(buf, s, s_len) == 0)
			ret = 1;
		free(buf);
	}
	return ret;
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
/* helpers */
/* format_helper_get_filename */
static char const * _format_helper_get_filename(Format * format)
{
	return format->filename;
}


/* format_helper_get_string_by_id */
static AsmString * _format_helper_get_string_by_id(Format * format, AsmId id)
{
	return code_get_string_by_id(format->code, id);
}


/* format_helper_set_function */
static int _format_helper_set_function(Format * format, int id,
		char const * name, off_t offset, ssize_t size)
{
	return code_set_function(format->code, id, name, offset, size);
}


/* format_helper_set_string */
static int _format_helper_set_string(Format * format, int id, char const * name,
		off_t offset, ssize_t size)
{
	return code_set_string(format->code, id, name, offset, size);
}


/* format_helper_decode */
static int _format_helper_decode(Format * format, char const * section,
		off_t offset, size_t size, off_t base)
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", 0x%lx, 0x%lx, 0x%lx)\n", __func__,
			section, offset, size, base);
#endif
	if((ret = code_decode_at(format->code, section, offset, size, base))
			!= 0)
		error_print("deasm");
	return ret;
}


/* format_helper_read */
static ssize_t _format_helper_read(Format * format, void * buf, size_t size)
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


/* format_helper_seek */
static off_t _format_helper_seek(Format * format, off_t offset, int whence)
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


/* format_helper_write */
static ssize_t _format_helper_write(Format * format, void const * buf,
		size_t size)
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
