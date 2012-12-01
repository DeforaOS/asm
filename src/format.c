/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm/format.h"
#include "format.h"
#include "../config.h"


/* AsmFormat */
/* private */
/* types */
struct _AsmFormat
{
	AsmFormatPluginHelper helper;
	Plugin * handle;
	AsmFormatPlugin * plugin;

	/* internal */
	/* file */
	char const * filename;
	FILE * fp;

	/* deassembly */
	AsmCode * code;
};


/* prototypes */
/* helpers */
static char const * _format_helper_get_filename(AsmFormat * format);
static void _format_helper_get_functions(AsmFormat * format,
		AsmFunction ** functions, size_t * functions_cnt);
static AsmSection * _format_helper_get_section_by_id(AsmFormat * format,
		AsmSectionId id);
static AsmString * _format_helper_get_string_by_id(AsmFormat * format,
		AsmStringId id);
static AsmFunction * _format_helper_set_function(AsmFormat * format,
		AsmFunctionId id, char const * name, off_t offset,
		ssize_t size);
static AsmSection * _format_helper_set_section(AsmFormat * format,
		AsmSectionId id, char const * name, off_t offset, ssize_t size,
		off_t base);
static AsmString * _format_helper_set_string(AsmFormat * format, AsmStringId id,
		char const * name, off_t offset, ssize_t size);

static int _format_helper_decode(AsmFormat * format, off_t offset, size_t size,
		off_t base, AsmArchInstructionCall ** calls, size_t * calls_cnt);
static ssize_t _format_helper_read(AsmFormat * format, void * buf, size_t size);
static off_t _format_helper_seek(AsmFormat * format, off_t offset, int whence);
static ssize_t _format_helper_write(AsmFormat * format, void const * buf,
		size_t size);


/* public */
/* functions */
#ifndef STANDALONE
/* format_new */
AsmFormat * format_new(char const * format)
{
	AsmFormat * f;
	Plugin * handle;
	AsmFormatPlugin * plugin;

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
#endif


/* format_delete */
void format_delete(AsmFormat * format)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	format_exit(format);
	plugin_delete(format->handle);
	object_delete(format);
}


/* accessors */
/* format_can_decode */
int format_can_decode(AsmFormat * format)
{
	return format->plugin->decode != NULL
		/* && format->plugin->decode_section != NULL */;
}


/* format_get_arch */
char const * format_get_arch(AsmFormat * format)
{
	if(format->plugin->detect == NULL)
		return NULL;
	return format->plugin->detect(format->plugin);
}


/* format_get_name */
char const * format_get_name(AsmFormat * format)
{
	return format->plugin->name;
}


/* useful */
/* format_decode */
int format_decode(AsmFormat * format, AsmCode * code, int raw)
{
	int ret;

	if(format->plugin->decode == NULL)
		return -error_set_code(1, "%s: %s", format_get_name(format),
				"Disassembly is not supported");
	format->code = code;
	ret = format->plugin->decode(format->plugin, raw);
	format->code = NULL;
	return ret;
}


/* format_decode_section */
int format_decode_section(AsmFormat * format, AsmCode * code, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	int ret;

	if(format->plugin->decode_section == NULL)
		return -error_set_code(1, "%s: %s", format_get_name(format),
				"Disassembly is not supported");
	if(section == NULL || section->id < 0)
		return -error_set_code(1, "%s: %s", format_get_name(format),
				"Invalid argument");
	format->code = code;
	ret = format->plugin->decode_section(format->plugin, section, calls,
			calls_cnt);
	format->code = NULL;
	return ret;
}


/* format_detect_arch */
char const * format_detect_arch(AsmFormat * format)
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
int format_exit(AsmFormat * format)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(format->plugin->helper != NULL && format->plugin->exit != NULL)
		ret = format->plugin->exit(format->plugin);
	format->plugin->helper = NULL;
	format->fp = NULL;
	format->filename = NULL;
	return ret;
}


/* format_function */
int format_function(AsmFormat * format, char const * function)
{
	if(format->plugin->function == NULL)
		return 0;
	return format->plugin->function(format->plugin, function);
}


/* format_init */
int format_init(AsmFormat * format, char const * arch, char const * filename,
		FILE * fp)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename,
			(void *)fp);
#endif
	if(format->plugin->helper != NULL)
		format_exit(format);
	format->filename = filename;
	format->fp = fp;
	format->plugin->helper = &format->helper;
	if(format->plugin->init != NULL && (ret = format->plugin->init(
					format->plugin, arch)) != 0)
		format->plugin->helper = NULL;
	return ret;
}


/* format_match */
int format_match(AsmFormat * format)
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
int format_section(AsmFormat * format, char const * section)
{
	if(format->plugin->section == NULL)
		return 0;
	return format->plugin->section(format->plugin, section);
}


/* private */
/* functions */
/* helpers */
/* format_helper_get_filename */
static char const * _format_helper_get_filename(AsmFormat * format)
{
	return format->filename;
}


/* format_helper_get_functions */
static void _format_helper_get_functions(AsmFormat * format,
		AsmFunction ** functions, size_t * functions_cnt)
{
	asmcode_get_functions(format->code, functions, functions_cnt);
}


/* format_helper_get_section_by_id */
static AsmSection * _format_helper_get_section_by_id(AsmFormat * format,
		AsmSectionId id)
{
	return asmcode_get_section_by_id(format->code, id);
}


/* format_helper_get_string_by_id */
static AsmString * _format_helper_get_string_by_id(AsmFormat * format,
		AsmStringId id)
{
	return asmcode_get_string_by_id(format->code, id);
}


/* format_helper_set_function */
static AsmFunction * _format_helper_set_function(AsmFormat * format,
		AsmFunctionId id, char const * name, off_t offset, ssize_t size)
{
	return asmcode_set_function(format->code, id, name, offset, size);
}


/* format_helper_set_section */
static AsmSection * _format_helper_set_section(AsmFormat * format,
		AsmSectionId id, char const * name, off_t offset, ssize_t size,
		off_t base)
{
	return asmcode_set_section(format->code, id, name, offset, size, base);
}


/* format_helper_set_string */
static AsmString * _format_helper_set_string(AsmFormat * format, AsmStringId id,
		char const * name, off_t offset, ssize_t size)
{
	return asmcode_set_string(format->code, id, name, offset, size);
}


/* format_helper_decode */
static int _format_helper_decode(AsmFormat * format, off_t offset, size_t size,
		off_t base, AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(0x%lx, 0x%lx, 0x%lx)\n", __func__, offset,
			size, base);
#endif
	if((ret = asmcode_decode_at(format->code, offset, size, base,
					calls, calls_cnt)) != 0)
		error_print("deasm");
	return ret;
}


/* format_helper_read */
static ssize_t _format_helper_read(AsmFormat * format, void * buf, size_t size)
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
static off_t _format_helper_seek(AsmFormat * format, off_t offset, int whence)
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
static ssize_t _format_helper_write(AsmFormat * format, void const * buf,
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
