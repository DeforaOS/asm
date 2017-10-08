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
#include <dirent.h>
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
	AsmFormatPluginDefinition * definition;
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
static AsmFunction * _format_helper_get_function_by_id(AsmFormat * format,
		AsmFunctionId id);
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
		AsmSectionId id, unsigned int flags, char const * name,
		off_t offset, ssize_t size, off_t base);
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

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
	if(format == NULL)
	{
		error_set_code(1, "%s", strerror(EINVAL));
		return NULL;
	}
	if((f = object_new(sizeof(*f))) == NULL)
		return NULL;
	if((f->handle = plugin_new(LIBDIR, PACKAGE, "format", format)) == NULL
			|| (f->definition = plugin_lookup(f->handle,
					"format_plugin")) == NULL)
	{
		if(f->handle != NULL)
			plugin_delete(f->handle);
		object_delete(f);
		return NULL;
	}
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
#endif


#ifndef STANDALONE
/* format_new_match */
AsmFormat * format_new_match(char const * filename, FILE * fp)
{
	char const path[] = LIBDIR "/" PACKAGE "/format";
	DIR * dir;
	struct dirent * de;
	size_t len;
#if defined(__APPLE__)
	char const ext[] = ".dylib";
#elif defined(__WIN32__)
	char const ext[] = ".dll";
#else
	char const ext[] = ".so";
#endif
	AsmFormat * flat = NULL;
	AsmFormat * format = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, filename);
#endif
	if((dir = opendir(path)) == NULL)
	{
		error_set_code(-errno, "%s: %s", path, strerror(errno));
		return NULL;
	}
	while((de = readdir(dir)) != NULL)
	{
		if((len = strlen(de->d_name)) < sizeof(ext))
			continue;
		if(strcmp(&de->d_name[len - sizeof(ext) + 1], ext) != 0)
			continue;
		de->d_name[len - sizeof(ext) + 1] = '\0';
		if((format = format_new(de->d_name)) == NULL)
			continue;
		if(format_init(format, NULL, filename, fp) == 0
				&& format_match(format) == 1)
			break;
		if(strcmp(de->d_name, "flat") == 0)
			flat = format;
		else
			format_delete(format);
		format = NULL;
	}
	closedir(dir);
	/* fallback on the "flat" format plug-in if necessary and available */
	if(format == NULL && flat != NULL)
		return flat;
	if(flat != NULL)
		format_delete(flat);
	return format;
}
#endif


/* format_delete */
void format_delete(AsmFormat * format)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* XXX may fail */
	format_exit(format);
	if(format->handle != NULL)
		plugin_delete(format->handle);
	object_delete(format);
}


/* accessors */
/* format_can_decode */
int format_can_decode(AsmFormat * format)
{
	return format->definition->decode != NULL
		/* && format->plugin->decode_section != NULL */;
}


/* format_get_arch */
char const * format_get_arch(AsmFormat * format)
{
	if(format->definition->detect == NULL)
		return NULL;
	return format->definition->detect(format->plugin);
}


/* format_get_description */
char const * format_get_description(AsmFormat * format)
{
	return format->definition->description;
}


/* format_get_name */
char const * format_get_name(AsmFormat * format)
{
	return format->definition->name;
}


/* useful */
/* format_decode */
int format_decode(AsmFormat * format, AsmCode * code, int raw)
{
	int ret;

	if(format->definition->decode == NULL)
		return -error_set_code(1, "%s: %s", format_get_name(format),
				"Disassembly is not supported");
	format->code = code;
	ret = format->definition->decode(format->plugin, raw);
	format->code = NULL;
	return ret;
}


/* format_decode_section */
int format_decode_section(AsmFormat * format, AsmCode * code, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	int ret;

	if(format->definition->decode_section == NULL)
		return -error_set_code(1, "%s: %s", format_get_name(format),
				"Disassembly is not supported");
	if(section == NULL || section->id < 0)
		return -error_set_code(1, "%s: %s", format_get_name(format),
				"Invalid argument");
	format->code = code;
	ret = format->definition->decode_section(format->plugin, section, calls,
			calls_cnt);
	format->code = NULL;
	return ret;
}


/* format_detect_arch */
char const * format_detect_arch(AsmFormat * format)
{
	if(format->definition->detect == NULL)
	{
		error_set_code(1, "%s: %s", format->definition->name,
				"Unable to detect the architecture");
		return NULL;
	}
	return format->definition->detect(format->plugin);
}


/* format_exit */
int format_exit(AsmFormat * format)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(format->plugin != NULL && format->definition->destroy != NULL)
		ret = format->definition->destroy(format->plugin);
	format->plugin = NULL;
	format->fp = NULL;
	format->filename = NULL;
	return ret;
}


/* format_function */
int format_function(AsmFormat * format, char const * function)
{
	if(format->definition->function == NULL)
		return 0;
	return format->definition->function(format->plugin, function);
}


/* format_guess_arch */
char const * format_guess_arch(AsmFormat * format, char const * hint)
{
	if(format->definition->guess == NULL)
		return NULL;
	return format->definition->guess(format->plugin, hint);
}


/* format_init */
int format_init(AsmFormat * format, char const * arch, char const * filename,
		FILE * fp)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename,
			(void *)fp);
#endif
	if(format->plugin != NULL)
		format_exit(format);
	format->filename = filename;
	format->fp = fp;
	if(format->definition->init != NULL
			&& (format->plugin = format->definition->init(
					&format->helper, arch)) == NULL)
		return -1;
	return 0;
}


/* format_match */
int format_match(AsmFormat * format)
{
	int ret = 0;
	char const * s = format->definition->signature;
	ssize_t s_len = format->definition->signature_len;
	char * buf = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(s_len > 0)
		if((buf = malloc(s_len)) == NULL)
			ret = error_set_code(-errno, "%s", strerror(errno));
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
	if(format->definition->section == NULL)
		return 0;
	return format->definition->section(format->plugin, section);
}


/* private */
/* functions */
/* helpers */
/* format_helper_get_filename */
static char const * _format_helper_get_filename(AsmFormat * format)
{
	return format->filename;
}


/* format_helper_get_function_by_id */
static AsmFunction * _format_helper_get_function_by_id(AsmFormat * format,
		AsmFunctionId id)
{
	return asmcode_get_function_by_id(format->code, id);
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
		AsmSectionId id, unsigned int flags, char const * name,
		off_t offset, ssize_t size, off_t base)
{
	return asmcode_set_section(format->code, id, flags,
			name, offset, size, base);
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
		return error_set_code(-errno, "%s: %s", format->filename,
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
	return error_set_code(-errno, "%s: %s", format->filename,
			strerror(errno));
}


/* format_helper_write */
static ssize_t _format_helper_write(AsmFormat * format, void const * buf,
		size_t size)
{
	if(fwrite(buf, size, 1, format->fp) == 1)
		return size;
	if(ferror(format->fp))
		return error_set_code(-errno, "%s: %s", format->filename,
				strerror(errno));
	if(feof(format->fp))
		return -error_set_code(1, "%s: %s", format->filename,
				"End of file reached");
	return -error_set_code(1, "%s: %s", format->filename, "Write error");
}
