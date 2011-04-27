/* $Id$ */
/* Copyright (c) 2011 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel asm */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License a published by
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
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "code.h"
#include "parser.h"
#include "asm.h"
#include "../config.h"


/* Asm */
/* private */
/* types */
struct _Asm
{
	Code * code;
};

typedef struct _AsmPluginDescription
{
	char const * name;
	char const * description;
} AsmPluginDescription;


/* constants */
#define APT_LAST	APT_FORMAT
#define APT_COUNT	(APT_LAST + 1)


/* variables */
static const AsmPluginDescription _asm_plugin_description[APT_COUNT] =
{
	{ "arch",	"architecture"	},
	{ "format",	"file format"	}
};


/* prototypes */
static char const * _asm_guess_arch(void);


/* functions */
/* asm_guess_arch */
static char const * _asm_guess_arch(void)
{
	static struct utsname uts;
	static int cached = 0;

	if(cached == 0)
	{
		if(uname(&uts) != 0)
		{
			error_set_code(1, "%s", strerror(errno));
			return NULL;
		}
		cached = 1;
	}
	return uts.machine;
}


/* public */
/* functions */
/* asm_new */
Asm * asm_new(char const * arch, char const * format)
{
	Asm * a;

	if((a = object_new(sizeof(*a))) == NULL)
		return NULL;
	if(arch == NULL)
		arch = _asm_guess_arch();
	if((a->code = code_new(arch, format)) == NULL)
	{
		object_delete(a);
		return NULL;
	}
	return a;
}


/* asm_delete */
void asm_delete(Asm * a)
{
	code_delete(a->code);
	object_delete(a);
}


/* accessors */
/* asm_get_arch */
Arch * asm_get_arch(Asm * a)
{
	return code_get_arch(a->code);
}


/* asm_get_arch_name */
char const * asm_get_arch_name(Asm * a)
{
	return code_get_arch_name(a->code);
}


/* asm_get_format */
Format * asm_get_format(Asm * a)
{
	return code_get_format(a->code);
}


/* asm_get_format_name */
char const * asm_get_format_name(Asm * a)
{
	return code_get_format_name(a->code);
}


/* useful */
/* asm_close */
int asm_close(Asm * a)
{
	return code_close(a->code);
}


/* asm_decode */
int asm_decode(Asm * a, char const * buffer, size_t size)
{
	return code_decode(a->code, buffer, size);
}


/* asm_decode_file */
int asm_decode_file(Asm * a, char const * filename, FILE * fp)
{
	int ret;

	if(fp != NULL)
		return code_decode_file(a->code, filename, fp);
	if((fp = fopen(filename, "r")) == NULL)
		return -error_set_code(1, "%s: %s", filename, strerror(errno));
	ret = code_decode_file(a->code, filename, fp);
	fclose(fp);
	return ret;
}


/* asm_parse */
int asm_parse(Asm * a, char const * infile, char const * outfile)
{
	int ret;

	if(asm_open(a, outfile) != 0)
		return -1;
	ret = parser(a->code, infile);
	if(ret != 0 && unlink(outfile) != 0)
		ret |= error_set_code(3, "%s: %s", outfile, strerror(errno));
	ret |= asm_close(a);
	return ret;
}


/* asm_open */
int asm_open(Asm * a, char const * outfile)
{
	return code_open(a->code, outfile);
}


/* asm_section */
int asm_section(Asm * a, char const * name)
{
	return code_section(a->code, name);
}


/* asm_function */
int asm_function(Asm * a, char const * name)
{
	return code_function(a->code, name);
}


/* asm_instruction */
int asm_instruction(Asm * a, char const * name, unsigned int operands_cnt, ...)
{
	ArchInstructionCall call;
	va_list ap;
	size_t i;
	ArchOperand * operand;

	memset(&call, 0, sizeof(call));
	call.name = name;
	if((call.operands_cnt = operands_cnt) != 0)
	{
		va_start(ap, operands_cnt);
		for(i = 0; i < 3 && i < operands_cnt; i++)
		{
			operand = va_arg(ap, ArchOperand *);
			memcpy(&call.operands[i], operand, sizeof(*operand));
		}
		va_end(ap);
	}
	return code_instruction(a->code, &call);
}


/* asm_plugin_list */
int asm_plugin_list(AsmPluginType type)
{
	AsmPluginDescription const * aspd;
	char * path;
	DIR * dir;
	struct dirent * de;
	size_t len;
	char const * sep = "";

	aspd = &_asm_plugin_description[type];
	fprintf(stderr, "%s%s%s", "Available ", aspd->description,
			" plug-ins:\n");
	len = strlen(LIBDIR) + 1 + strlen(PACKAGE) + 1 + strlen(aspd->name) + 1;
	if((path = malloc(len)) == NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		fputc('\n', stderr);
		return 1;
	}
	snprintf(path, len, "%s/%s/%s", LIBDIR, PACKAGE, aspd->name);
	if((dir = opendir(path)) == NULL)
	{
		error_set_code(1, "%s: %s", path, strerror(errno));
		fputc('\n', stderr);
		free(path);
		return 1;
	}
	while((de = readdir(dir)) != NULL)
	{
		if((len = strlen(de->d_name)) < 4)
			continue;
		if(strcmp(".so", &de->d_name[len - 3]) != 0)
			continue;
		de->d_name[len - 3] = '\0';
		fprintf(stderr, "%s%s", sep, de->d_name);
		sep = ", ";
	}
	free(path);
	closedir(dir);
	fputc('\n', stderr);
	return 0;
}
