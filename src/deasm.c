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
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm.h"
#include "../config.h"


/* constants */
#ifndef PREFIX
# define PREFIX		"/usr/local"
#endif
#ifndef LIBDIR
# define LIBDIR		PREFIX "/lib"
#endif


/* macros */
#define max(a, b)	((a) > (b) ? (a) : (b))


/* deasm */
/* private */
/* types */
typedef struct _DeasmFormat
{
	Plugin * plugin;
	FormatPlugin * format;
} DeasmFormat;

/* XXX ugly */
#define _Deasm _Format
typedef struct _Deasm
{
	char const * arch;

	FormatPluginHelper helper;
	DeasmFormat * format;
	size_t format_cnt;

	char const * filename;
	FILE * fp;
} Deasm;


/* prototypes */
static int _deasm(char const * arch, char const * format,
		char const * filename);
static int _deasm_buffer(char const * arch, char const * format,
		char const * buffer, size_t size);
static int _deasm_error(char const * message, int ret);
static int _deasm_string(char const * arch, char const * format,
		char const * string);
static int _deasm_list(void);

/* helper */
static ssize_t _deasm_helper_read(Format * format, void * buf, size_t size);
static off_t _deasm_helper_seek(Format * format, off_t offset, int whence);

/* format */
static DeasmFormat const * _deasm_format_open(Deasm * deasm,
		char const * format);
static int _deasm_format_open_all(Deasm * deasm);
static char const * _deasm_format_detect(DeasmFormat const * format);
static void _deasm_format_close(DeasmFormat const * format);
static void _deasm_format_close_all(Deasm * deasm);

static int _usage(void);


/* functions */
/* deasm */
static int _deasm_do_format(Deasm * deasm, char const * format);
static int _deasm_do(Deasm * deasm);
static int _deasm_do_callback(Deasm * deasm, FormatPlugin * format);

static int _deasm(char const * arch, char const * format,
		char const * filename)
{
	int ret = 1;
	Deasm deasm;

	deasm.arch = arch;
	memset(&deasm.helper, 0, sizeof(deasm.helper));
	deasm.helper.format = &deasm;
	deasm.helper.read = _deasm_helper_read;
	deasm.helper.seek = _deasm_helper_seek;
	deasm.format = NULL;
	deasm.format_cnt = 0;
	if((deasm.fp = fopen(filename, "r")) == NULL)
		return -_deasm_error(filename, 1);
	deasm.filename = filename;
	if(format != NULL)
		ret = _deasm_do_format(&deasm, format);
	else
		ret = _deasm_do(&deasm);
	fclose(deasm.fp);
	_deasm_format_close_all(&deasm);
	if(ret != 0)
		error_print("deasm");
	return ret;
}

static int _deasm_do_format(Deasm * deasm, char const * format)
{
	int ret;
	size_t i;
	DeasmFormat const * df;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
	/* check if the plug-in is already opened */
	for(i = 0; i < deasm->format_cnt; i++)
		if(strcmp(deasm->format[i].format->name, format) == 0)
			break;
	if(i < deasm->format_cnt)
		df = &deasm->format[i];
	else if((df = _deasm_format_open(deasm, format)) == NULL)
		return -1;
	/* disassemble */
	ret = _deasm_do_callback(deasm, df->format);
	return ret;
}

static int _deasm_do(Deasm * deasm)
{
	int ret = 1;
	size_t i;
	size_t s = 0;
	char * buf;
	FormatPlugin * format;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	_deasm_format_open_all(deasm);
	for(i = 0; i < deasm->format_cnt; i++)
		s = max(s, deasm->format[i].format->signature_len);
	if((buf = malloc(s)) == NULL)
		return -_deasm_error(deasm->filename, 1);
	if(fread(buf, 1, s, deasm->fp) != s)
	{
		if(ferror(deasm->fp))
			ret = -_deasm_error(deasm->filename, 1);
		else
			ret = _deasm_do_format(deasm, "flat");
	}
	else
	{
		for(i = 0; i < deasm->format_cnt; i++)
		{
			format = deasm->format[i].format;
			if(format->signature_len == 0)
				continue;
			if(memcmp(format->signature, buf, format->signature_len)
					!= 0)
				continue;
			if(deasm->arch == NULL)
				deasm->arch = _deasm_format_detect(
						&deasm->format[i]);
			if(deasm->arch == NULL)
				continue;
			ret = _deasm_do_callback(deasm, format);
			break;
		}
		if(i == deasm->format_cnt)
			ret = _deasm_do_format(deasm, "flat");
	}
	free(buf);
	return ret;
}

static int _deasm_do_callback(Deasm * deasm, FormatPlugin * format)
{
	int ret;
	Asm * a;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format->name);
#endif
	if(deasm->arch == NULL)
	{
		if(format->detect == NULL)
			return -error_set_code(1, "%s: %s", deasm->filename,
					"Unable to detect the architecture");
		if((deasm->arch = format->detect(format)) == NULL)
			return -1;
	}
	if((a = asm_new(deasm->arch, format->name)) == NULL)
		return -error_print("deasm");
	printf("\n%s: %s-%s\n", deasm->filename, format->name, asm_get_arch(a));
	ret = asm_open_deassemble(a, deasm->filename);
	asm_delete(a);
	return ret;
}


/* deasm_buffer */
static int _deasm_buffer(char const * arch, char const * format,
		char const * buffer, size_t size)
{
	Asm * a;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if((a = asm_new(arch, format)) == NULL)
		return -1;
	if(asm_deassemble(a, buffer, size) != 0)
		error_print("deasm");
	asm_delete(a);
	return 0;
}


/* deasm_error */
static int _deasm_error(char const * message, int ret)
{
	fputs("deasm: ", stderr);
	perror(message);
	return ret;
}


/* deasm_string */
static int _string_hex2bin(int c);
static int _string_ishex(int c);

static int _deasm_string(char const * arch, char const * format,
		char const * string)
{
	int ret;
	unsigned char * str = (unsigned char *)string;
	size_t len = strlen(string);
	char * s;
	size_t i;
	size_t j;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", \"%s\", \"%s\")\n", __func__, arch,
			format, string);
#endif
	if((s = malloc(len + 1)) == NULL)
		return -error_set_print("deasm", 1, "%s", strerror(errno));
	for(i = 0, j = 0; i < len; i++)
	{
		if(str[i] != '\\')
			s[j++] = str[i];
		else if(str[i + 1] != 'x') /* "\\" */
			s[j++] = str[++i];
		else if(i + 3 < len && _string_ishex(str[i + 2])
				&& _string_ishex(str[i + 3])) /* "\xHH" */
		{
			s[j++] = (_string_hex2bin(str[i + 2]) << 4)
				| _string_hex2bin(str[i + 3]);
			i += 3;
		}
	}
	s[j] = '\0'; /* not really necessary */
	ret = _deasm_buffer(arch, format, s, j);
	free(s);
	return ret;
}

static int _string_hex2bin(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

static int _string_ishex(int c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
		|| (c >= 'A' || c <= 'F');
}


/* deasm_list */
static int _deasm_list(void)
{
	Deasm deasm;
	size_t i;
	char const * sep = "";

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	memset(&deasm, 0, sizeof(deasm));
	asm_plugin_list(APT_ARCH);
	_deasm_format_open_all(&deasm);
	fputs("\nAvailable format plug-ins:\n", stderr);
	for(i = 0; i < deasm.format_cnt; i++)
	{
		if(deasm.format[i].format->decode == NULL)
			continue;
		fprintf(stderr, "%s%s", sep, deasm.format[i].format->name);
		sep = ", ";
	}
	fputc('\n', stderr);
	_deasm_format_close_all(&deasm);
	return 0;
}



/* helper */
/* deasm_helper_read */
static ssize_t _deasm_helper_read(Format * format, void * buf, size_t size)
{
	if(fread(buf, size, 1, format->fp) == 1)
		return size;
	return -error_set_code(1, "%s: %s", format->filename, strerror(errno));
}


/* deasm_helper_seek */
static off_t _deasm_helper_seek(Format * format, off_t offset, int whence)
{
	if(fseek(format->fp, offset, whence) != 0)
		return -error_set_code(1, "%s: %s", format->filename,
				strerror(errno));;
	return ftello(format->fp);
}


/* format */
/* deasm_format_open */
static DeasmFormat const * _deasm_format_open(Deasm * deasm,
		char const * format)
{
	DeasmFormat * p;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
	if((p = realloc(deasm->format, sizeof(*p) * (deasm->format_cnt + 1)))
			== NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		return NULL;
	}
	deasm->format = p;
	p = &deasm->format[deasm->format_cnt];
	if((p->plugin = plugin_new(LIBDIR, PACKAGE, "format", format)) == NULL)
		return NULL;
	if((p->format = plugin_lookup(p->plugin, "format_plugin")) == NULL)
	{
		plugin_delete(p->plugin);
		return NULL;
	}
	if(p->format->decode == NULL)
	{
		error_set_code(1, "%s: %s", format,
				"Does not support disassembly");
		plugin_delete(p->plugin);
		return NULL;
	}
	p->format->helper = &deasm->helper;
	deasm->format_cnt++;
	return p;
}


/* deasm_format_open_all */
static int _deasm_format_open_all(Deasm * deasm)
{
	char const path[] = LIBDIR "/" PACKAGE "/format";
	DIR * dir;
	struct dirent * de;
	size_t len;
	char const ext[] = ".so";

	if((dir = opendir(path)) == NULL)
		return -error_set_print("deasm", 1, "%s: %s", path, strerror(
					errno));
	while((de = readdir(dir)) != NULL)
	{
		if((len = strlen(de->d_name)) < 4)
			continue;
		if(strcmp(&de->d_name[len - sizeof(ext) + 1], ext) != 0)
			continue;
		de->d_name[len - sizeof(ext) + 1] = '\0';
		if(_deasm_format_open(deasm, de->d_name) == NULL)
			error_print("deasm");
	}
	closedir(dir);
	return 0;
}


/* deasm_format_detect */
static char const * _deasm_format_detect(DeasmFormat const * format)
{
	if(format->format->detect == NULL)
		return NULL;
	return format->format->detect(format->format);
}


/* deasm_format_close */
static void _deasm_format_close(DeasmFormat const * format)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() \"%s\"\n", __func__, format->format->name);
#endif
	plugin_delete(format->plugin);
}


/* deasm_format_close_all */
static void _deasm_format_close_all(Deasm * deasm)
{
	size_t i;

	for(i = 0; i < deasm->format_cnt; i++)
		_deasm_format_close(&deasm->format[i]);
	free(deasm->format);
	deasm->format = NULL;
	deasm->format_cnt = 0;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: deasm [-a arch][-f format] filename\n"
"       deasm [-a arch][-f format] -s string\n"
"       deasm -l\n", stderr);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;
	char const * arch = NULL;
	char const * format = NULL;
	char const * string = NULL;

	while((o = getopt(argc, argv, "a:f:ls:")) != -1)
		switch(o)
		{
			case 'a':
				arch = optarg;
				break;
			case 'f':
				format = optarg;
				break;
			case 'l':
				return _deasm_list();
			case 's':
				string = optarg;
				break;
			default:
				return _usage();
		}
	if(optind == argc && string != NULL)
		return _deasm_string(arch, format, string);
	else if(optind + 1 == argc && string == NULL)
		return (_deasm(arch, format, argv[optind]) == 0) ? 0 : 2;
	return _usage();
}
