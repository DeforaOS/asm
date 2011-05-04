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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm/asm.h"
#include "../config.h"


/* as */
/* private */
/* constants */
# define ASM_FILENAME_DEFAULT "a.out"


/* functions */
/* asm */
static int _asm(AsmPrefs * prefs, char const * arch, char const * format,
		char const * infile, char const * outfile)
{
	int ret = 0;
	Asm * a;

	if((a = asm_new(arch, format)) == NULL)
		return error_print(PACKAGE);
	if(asm_assemble(a, prefs, infile, outfile) != 0)
		ret = error_print(PACKAGE);
	asm_delete(a);
	return ret;
}


/* usage */
static unsigned int _usage(void)
{
	fputs("Usage: asm [-D name][-a arch][-f format][-o file] file\n"
"       asm -l\n"
"  -D	Set a variable in the pre-processor\n"
"  -a	Target architecture\n"
"  -f	Target file format\n"
"  -o	Filename to use for output (default: " ASM_FILENAME_DEFAULT ")\n"
"  -l	List available architectures and formats\n", stderr);
	return 1;
}


/* public */
/* main */
static int _main_add_define(AsmPrefs * prefs, char * define);

int main(int argc, char * argv[])
{
	int ret;
	AsmPrefs prefs;
	int o;
	char * outfile = ASM_FILENAME_DEFAULT;
	char const * arch = NULL;
	char const * format = NULL;

	memset(&prefs, 0, sizeof(prefs));
	while((o = getopt(argc, argv, "a:D:f:o:l")) != -1)
		switch(o)
		{
			case 'a':
				arch = optarg;
				break;
			case 'D':
				if(_main_add_define(&prefs, optarg) != 0)
					return 2;
				break;
			case 'f':
				format = optarg;
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'l':
				o = 0;
				if(asm_plugin_list(APT_ARCH) != 0)
					o = error_print(PACKAGE);
				else
					putchar('\n');
				if(asm_plugin_list(APT_FORMAT) != 0)
					o = error_print(PACKAGE);
				return (o == 0) ? 0 : 2;
			default:
				return _usage();
		}
	if(optind + 1 != argc)
		return _usage();
	ret = _asm(&prefs, arch, format, argv[optind], outfile);
	free(prefs.defines);
	return (ret == 0) ? 0 : 2;
}

static int _main_add_define(AsmPrefs * prefs, char * define)
{
	char ** p;
	char * value;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, define);
#endif
	if(strlen(define) == 0)
		return -_usage();
	value = strtok(define, "=");
	if((p = realloc(prefs->defines, sizeof(*p) * (prefs->defines_cnt + 1)))
			== NULL)
		return -error_set_print(PACKAGE, 1, "%s", strerror(errno));
	prefs->defines = p;
	prefs->defines[prefs->defines_cnt++] = define;
	return 0;
}
