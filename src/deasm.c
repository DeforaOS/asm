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
static int _deasm(char const * arch, char const * format, char const * filename,
		int raw);
static int _deasm_buffer(char const * arch, char const * buffer, size_t size);
static int _deasm_string(char const * arch, char const * format,
		char const * string);
static int _deasm_list(void);

static int _usage(void);


/* functions */
/* deasm */
static int _deasm_section(AsmCode * code, AsmSection * section);

static int _deasm(char const * arch, char const * format, char const * filename,
		int raw)
{
	int ret;
	Asm * a;
	AsmCode * code;
	AsmSection * sections;
	size_t sections_cnt;
	size_t i;

	if((a = asm_new(arch, format)) == NULL)
		return -error_print("deasm");
	if((code = asm_open_deassemble(a, filename, raw)) == NULL)
		error_print("deasm");
	else
	{
		printf("%s: %s-%s\n", filename, asm_get_format(a),
				asm_get_arch(a));
		asmcode_get_sections(code, &sections, &sections_cnt);
		for(i = 0; i < sections_cnt; i++)
			if((ret = _deasm_section(code, &sections[i])) != 0)
				break;
		asm_close(a);
	}
	asm_delete(a);
	return ret;
}

static int _deasm_section(AsmCode * code, AsmSection * section)
{
	ArchDescription * description;
	size_t size;
	ArchInstructionCall * calls = NULL;
	size_t calls_cnt = 0;
	size_t i;

	printf("\nDisassembly of section %s:\n", section->name);
	if(asmcode_decode_section(code, section, &calls, &calls_cnt) != 0)
	{
		error_print("deasm");
		return -1;
	}
	description = asmcode_get_arch_description(code);
	size = (description != NULL) ? description->address_size : 32;
	switch(size)
	{
		case 64:
			printf("\n%016lx:\n", section->base);
			break;
		case 20:
			printf("\n%05lx:\n", section->base);
			break;
		case 32:
		default:
			printf("\n%08lx:\n", section->base);
			break;
	}
	for(i = 0; i < calls_cnt; i++)
		asmcode_print(code, &calls[i]);
	free(calls);
	return 0;
}


/* deasm_buffer */
static int _deasm_buffer(char const * arch, char const * buffer, size_t size)
{
	Asm * a;
	AsmCode * code;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if((a = asm_new(arch, NULL)) == NULL)
		return -1;
	if((code = asm_deassemble(a, buffer, size, NULL, NULL)) == NULL)
		error_print("deasm");
	else
	{
		/* FIXME implement */
	}
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
	ret = _deasm_buffer(arch, s, j);
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
	asm_plugin_list(APT_ARCH, 1);
	asm_plugin_list(APT_FORMAT, 1);
	return 0;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: deasm [-a arch][-f format][-D] filename\n"
"       deasm [-a arch] -s string\n"
"       deasm -l\n"
"  -a	Force the given architecture\n"
"  -f	Force the given file format\n"
"  -D	Deassemble all sections\n"
"  -l	List all the architectures and file formats available\n", stderr);
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
	int raw = 0;

	while((o = getopt(argc, argv, "a:f:ls:D")) != -1)
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
			case 'D':
				raw = 1;
				break;
			default:
				return _usage();
		}
	if(optind == argc && string != NULL)
		return _deasm_string(arch, format, string);
	else if(optind + 1 == argc && string == NULL)
		return (_deasm(arch, format, argv[optind], raw) == 0) ? 0 : 2;
	return _usage();
}
