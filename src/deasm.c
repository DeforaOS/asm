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


/* deasm */
/* private */
/* prototypes */
static int _deasm(char const * arch, char const * format,
		char const * filename);
static int _deasm_buffer(char const * arch, char const * format,
		char const * buffer, size_t size);
static int _deasm_string(char const * arch, char const * format,
		char const * string);
static int _deasm_list(void);

static int _usage(void);


/* functions */
/* deasm */
static int _deasm(char const * arch, char const * format, char const * filename)
{
	int ret;
	Asm * a;

	if((a = asm_new(arch, format)) == NULL)
		return -error_print("deasm");
	if((ret = asm_open_deassemble(a, filename)) != 0)
		error_print("deasm");
	else
		asm_close(a);
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
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	asm_plugin_list(APT_ARCH);
	asm_plugin_list(APT_FORMAT);
	return 0;
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
