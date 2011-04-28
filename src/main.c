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
#include <stdio.h>
#include "Asm/asm.h"
#include "common.h"
#include "../config.h"


/* as */
/* private */
/* constants */
# define ASM_FILENAME_DEFAULT "a.out"


/* functions */
/* asm */
static int _asm(char const * arch, char const * format, char const * infile,
		char const * outfile)
{
	int ret = 0;
	Asm * a;

	if((a = asm_new(arch, format)) == NULL)
		return error_print(PACKAGE);
	if(asm_assemble(a, infile, outfile) != 0)
		ret = error_print(PACKAGE);
	asm_delete(a);
	return ret;
}


/* usage */
static unsigned int _usage(void)
{
	fputs("Usage: as [-a arch][-f format][-o file] file\n"
"       as -l\n"
"  -a	target architecture\n"
"  -f	target file format\n"
"  -o	filename to use for output (default: " ASM_FILENAME_DEFAULT ")\n"
"  -l	list available architectures and formats\n", stderr);
	return 1;
}


/* public */
/* main */
int main(int argc, char * argv[])
{
	int o;
	char * outfile = ASM_FILENAME_DEFAULT;
	char const * arch = NULL;
	char const * format = NULL;

	while((o = getopt(argc, argv, "a:f:o:l")) != -1)
	{
		switch(o)
		{
			case 'a':
				arch = optarg;
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
	}
	if(optind + 1 != argc)
		return _usage();
	return (_asm(arch, format, argv[optind], outfile) == 0) ? 0 : 2;
}
