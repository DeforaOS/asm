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


/* as */
/* private */
/* types */
struct _As
{
	Code * code;
};

typedef struct _AsPluginDescription
{
	char const * name;
	char const * description;
} AsPluginDescription;


/* constants */
#define ASPT_LAST	ASPT_FORMAT
#define ASPT_COUNT	(ASPT_LAST + 1)


/* variables */
static const AsPluginDescription _as_plugin_description[ASPT_COUNT] =
{
	{ "arch",	"architecture"	},
	{ "format",	"file format"	}
};


/* prototypes */
static char const * _as_guess_arch(void);


/* functions */
/* as_guess_arch */
static char const * _as_guess_arch(void)
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
/* as_new */
As * as_new(char const * arch, char const * format)
{
	As * as;

	/* FIXME add signatures to the format plug-in (for disas too) */
	if((as = object_new(sizeof(*as))) == NULL)
		return NULL;
	if(arch == NULL)
		arch = _as_guess_arch();
	if((as->code = code_new(arch, format)) == NULL)
	{
		object_delete(as);
		return NULL;
	}
	return as;
}


/* as_delete */
void as_delete(As * as)
{
	code_delete(as->code);
	object_delete(as);
}


/* accessors */
/* as_get_arch */
Arch * as_get_arch(As * as)
{
	return code_get_arch(as->code);
}


/* as_get_arch_name */
char const * as_get_arch_name(As * as)
{
	return code_get_arch_name(as->code);
}


/* as_get_format */
Format * as_get_format(As * as)
{
	return code_get_format(as->code);
}


/* as_get_format_name */
char const * as_get_format_name(As * as)
{
	return code_get_format_name(as->code);
}


/* useful */
/* as_close */
int as_close(As * as)
{
	return code_close(as->code);
}


/* as_decode */
ArchInstruction * as_decode(As * as, char const * buffer, size_t * size)
{
	return code_decode(as->code, buffer, size);
}


/* as_parse */
int as_parse(As * as, char const * infile, char const * outfile)
{
	int ret;

	if(as_open(as, outfile) != 0)
		return -1;
	ret = parser(as->code, infile);
	if(ret != 0 && unlink(outfile) != 0)
		ret |= error_set_code(3, "%s: %s", outfile, strerror(errno));
	ret |= as_close(as);
	return ret;
}


/* as_open */
int as_open(As * as, char const * outfile)
{
	return code_open(as->code, outfile);
}


/* as_section */
int as_section(As * as, char const * name)
{
	return code_section(as->code, name);
}


/* as_function */
int as_function(As * as, char const * name)
{
	return code_function(as->code, name);
}


/* as_instruction */
int as_instruction(As * as, char const * name, unsigned int operands_cnt, ...)
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
	return code_instruction(as->code, &call);
}


/* as_plugin_list */
int as_plugin_list(AsPluginType type)
{
	AsPluginDescription const * aspd;
	char * path;
	DIR * dir;
	struct dirent * de;
	size_t len;
	char const * sep = "";

	aspd = &_as_plugin_description[type];
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
