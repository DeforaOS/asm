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
#include "../config.h"


/* Asm */
/* private */
/* types */
struct _Asm
{
	char * arch;
	char * format;

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

static const AsmPluginDescription _asm_plugin_description[APT_COUNT] =
{
	{ "arch",	"architecture"	},
	{ "format",	"file format"	}
};


/* prototypes */
static char const * _asm_guess_arch(void);

static int _asm_open(Asm * a, char const * outfile);


/* public */
/* functions */
/* asm_new */
Asm * asm_new(char const * arch, char const * format)
{
	Asm * a;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", \"%s\")\n", __func__, arch, format);
#endif
	if((a = object_new(sizeof(*a))) == NULL)
		return NULL;
	a->arch = (arch != NULL) ? string_new(arch) : NULL;
	a->format = (format != NULL) ? string_new(format) : NULL;
	a->code = NULL;
	if((arch != NULL && a->arch == NULL)
			|| (format != NULL && a->format == NULL))
	{
		object_delete(a);
		return NULL;
	}
	return a;
}


/* asm_delete */
void asm_delete(Asm * a)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(a->code != NULL)
		code_delete(a->code);
	string_delete(a->format);
	string_delete(a->arch);
	object_delete(a);
}


/* accessors */
/* asm_get_arch */
char const * asm_get_arch(Asm * a)
{
	return a->arch;
}


/* asm_get_format */
char const * asm_get_format(Asm * a)
{
	return a->format;
}


/* asm_set_arch */
int asm_set_arch(Asm * a, char const * arch)
{
	char * p;

	if((p = string_new(arch)) == NULL)
		return -1;
	string_delete(a->arch);
	a->arch = p;
	return 0;
}


/* asm_set_format */
int asm_set_format(Asm * a, char const * format)
{
	char * p;

	if((p = string_new(format)) == NULL)
		return -1;
	string_delete(a->format);
	a->format = p;
	return 0;
}


/* asm_set_function */
int asm_set_function(Asm * a, char const * name, off_t offset, ssize_t size)
{
	return code_set_function(a->code, -1, name, offset, size);
}


/* asm_set_section */
int asm_set_section(Asm * a, char const * name, off_t offset, ssize_t size)
{
	/* FIXME fully implement */
	return code_section(a->code, name);
}


/* useful */
/* asm_assemble */
int asm_assemble(Asm * a, AsmPrefs * prefs, char const * infile,
		char const * outfile)
{
	int ret;

	if(_asm_open(a, outfile) != 0)
		return -1;
	ret = parser(prefs, a->code, infile);
	if(ret != 0 && unlink(outfile) != 0)
		ret |= error_set_code(3, "%s: %s", outfile, strerror(errno));
	ret |= asm_close(a);
	return ret;
}


/* asm_close */
int asm_close(Asm * a)
{
	int ret;

	if(a->code == NULL)
		return -error_set_code(1, "%s", "No file opened");
	ret = code_close(a->code);
	a->code = NULL;
	return ret;
}


/* asm_deassemble */
int asm_deassemble(Asm * a, char const * buffer, size_t size)
{
	int ret;

	if(_asm_open(a, NULL) != 0)
		return -1;
	ret = code_decode_buffer(a->code, buffer, size);
	asm_close(a);
	return ret;
}


/* asm_function */
int asm_function(Asm * a, char const * name)
{
	return code_function(a->code, name);
}


/* asm_guess_arch */
int asm_guess_arch(Asm * a)
{
	char const * arch;

	if((arch = _asm_guess_arch()) == NULL)
		return -1;
	return asm_set_arch(a, arch);
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


/* asm_open_assemble */
int asm_open_assemble(Asm * a, char const * outfile)
{
	return _asm_open(a, outfile);
}


/* asm_open_deassemble */
int asm_open_deassemble(Asm * a, char const * filename, int raw)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, filename);
#endif
	if(a->code != NULL)
		return -error_set_code(1, "%s: Operation in progress",
				code_get_filename(a->code));
	if((a->code = code_new_file(a->arch, a->format, filename)) == NULL)
		return -1;
	if(code_decode(a->code, raw) != 0)
		return -1;
	return 0;
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


/* private */
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


/* asm_open */
static int _asm_open(Asm * a, char const * outfile)
{
	char const * arch = a->arch;
	char const * format = a->format;

	if(arch == NULL && (arch = _asm_guess_arch()) == NULL)
		return -1;
	if(a->code != NULL)
		return -error_set_code(1, "%s: Operation in progress",
				code_get_filename(a->code));
	if((a->code = code_new(arch, format)) == NULL)
		return -1;
	if(outfile == NULL)
		return 0;
	return code_open(a->code, outfile);
}
