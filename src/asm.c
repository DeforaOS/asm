/* $Id$ */
/* Copyright (c) 2011-2017 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel Asm */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License a published by
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
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm/asm.h"
#include "arch.h"
#include "code.h"
#include "format.h"
#include "parser.h"
#include "../config.h"


/* Asm */
/* private */
/* types */
struct _Asm
{
#if 1 /* FIXME probably useless now */
	char * arch;
	char * format;
#endif

	AsmCode * code;
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
static int _asm_open_file(Asm * a, char const * outfile, FILE * fp);


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
		asmcode_delete(a->code);
	string_delete(a->format);
	string_delete(a->arch);
	object_delete(a);
}


/* accessors */
/* asm_get_arch */
char const * asm_get_arch(Asm * a)
{
	if(a->code != NULL)
		return asmcode_get_arch(a->code);
	return a->arch;
}


/* asm_get_format */
char const * asm_get_format(Asm * a)
{
	if(a->code != NULL)
		return asmcode_get_format(a->code);
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
AsmFunction * asm_set_function(Asm * a, char const * name, off_t offset,
		ssize_t size)
{
	return asmcode_set_function(a->code, -1, name, offset, size);
}


/* asm_set_section */
AsmSection * asm_set_section(Asm * a, unsigned int flags, char const * name,
		off_t offset, ssize_t size, off_t base)
{
	return asmcode_set_section(a->code, -1, flags, name, offset, size,
			base);
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


/* asm_assemble_string */
int asm_assemble_string(Asm * a, AsmPrefs * prefs, char const * outfile,
		char const * string)
{
	int ret;

	/* FIXME should also support returning a buffer */
	if(outfile == NULL)
	{
		if(_asm_open_file(a, NULL, stdout) != 0)
			return -1;
	}
	else if(_asm_open(a, outfile) != 0)
		return -1;
	ret = parser_string(prefs, a->code, string);
	ret |= asm_close(a);
	return ret;
}


/* asm_close */
int asm_close(Asm * a)
{
	int ret;

	if(a->code == NULL)
		return -error_set_code(1, "%s", "No file opened");
	ret = asmcode_close(a->code);
	a->code = NULL;
	return ret;
}


/* asm_deassemble */
AsmCode * asm_deassemble(Asm * a, char const * buffer, size_t size,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	if(_asm_open(a, NULL) != 0)
		return NULL;
	if(asmcode_decode_buffer(a->code, buffer, size, calls, calls_cnt) != 0)
		return NULL;
	return a->code;
}


/* asm_function */
int asm_function(Asm * a, char const * name)
{
	return asmcode_function(a->code, name);
}


/* asm_guess_arch */
int asm_guess_arch(Asm * a)
{
	char const * arch;

	if((arch = _asm_guess_arch()) == NULL)
		return -1;
	return asm_set_arch(a, arch);
}


/* asm_guess_format */
int asm_guess_format(Asm * a)
{
	int ret = -1;
	AsmCode * code;
	char const * format;

	if((code = asmcode_new(a->arch, a->format)) == NULL)
		return -1;
	if((format = asmcode_get_format(code)) != NULL
			&& asm_set_format(a, format) == 0)
		ret = 0;
	asmcode_delete(code);
	return ret;
}


/* asm_instruction */
int asm_instruction(Asm * a, char const * name, unsigned int operands_cnt, ...)
{
	AsmArchInstructionCall call;
	va_list ap;
	size_t i;
	AsmArchOperand * operand;

	memset(&call, 0, sizeof(call));
	call.name = name;
	if((call.operands_cnt = operands_cnt) != 0)
	{
		va_start(ap, operands_cnt);
		for(i = 0; i < sizeof(call.operands) / sizeof(*call.operands)
				&& i < operands_cnt; i++)
		{
			operand = va_arg(ap, AsmArchOperand *);
			memcpy(&call.operands[i], operand, sizeof(*operand));
		}
		va_end(ap);
	}
	return asmcode_instruction(a->code, &call);
}


/* asm_open_assemble */
int asm_open_assemble(Asm * a, char const * outfile)
{
	return (outfile != NULL) ? _asm_open(a, outfile)
		: _asm_open_file(a, NULL, stdout);
}


/* asm_open_deassemble */
AsmCode * asm_open_deassemble(Asm * a, char const * filename, int raw)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, filename);
#endif
	if(a->code != NULL)
	{
		error_set_code(1, "%s: Operation in progress",
				asmcode_get_filename(a->code));
		return NULL;
	}
	if((a->code = asmcode_new_file(a->arch, a->format, filename)) == NULL
			|| asmcode_decode(a->code, raw) != 0)
		return NULL;
	return a->code;
}


/* asm_plugin_list */
int asm_plugin_list(AsmPluginType type, int decode)
{
	AsmPluginDescription const * aspd;
	char * path;
	DIR * dir;
	struct dirent * de;
	size_t len;
	char const * sep = "";
#if defined(__APPLE__)
	char const ext[] = ".dylib";
#elif defined(__WIN32__)
	char const ext[] = ".dll";
#else
	char const ext[] = ".so";
#endif
	char const * description;
	AsmArch * arch;
	AsmFormat * format;

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
		if((len = strlen(de->d_name)) < sizeof(ext))
			continue;
		if(strcmp(ext, &de->d_name[len - sizeof(ext) + 1]) != 0)
			continue;
		de->d_name[len - sizeof(ext) + 1] = '\0';
		if(type == APT_ARCH && (arch = arch_new(de->d_name)) != NULL
				&& (decode == 0 || arch_can_decode(arch)))
		{
			description = arch_get_description(arch);
			fprintf(stderr, "%s%s (%s)", sep, arch_get_name(arch),
					description);
			arch_delete(arch);
		}
		else if(type == APT_FORMAT && (format = format_new(de->d_name))
				!= NULL && (decode == 0
					|| format_can_decode(format)))
		{
			description = format_get_description(format);
			fprintf(stderr, "%s%s (%s)", sep,
					format_get_name(format), description);
			format_delete(format);
		}
		else
			continue;
		sep = "\n";
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
				asmcode_get_filename(a->code));
	if((a->code = asmcode_new(arch, format)) == NULL)
		return -1;
	if(outfile == NULL)
		return 0;
	return asmcode_open(a->code, outfile);
}


/* asm_open_file */
static int _asm_open_file(Asm * a, char const * outfile, FILE * fp)
{
	char const * arch = a->arch;
	char const * format = a->format;
	char const * filename;

	if(arch == NULL && (arch = _asm_guess_arch()) == NULL)
		return -1;
	if(a->code != NULL)
	{
		filename = asmcode_get_filename(a->code);
		return -error_set_code(1, "%s%sOperation in progress",
				(filename != NULL) ? filename : "",
				(filename != NULL) ? ": " : "");
	}
	if((a->code = asmcode_new(arch, format)) == NULL)
		return -1;
	return asmcode_open_file(a->code, outfile, fp);
}
