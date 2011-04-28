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
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "arch.h"
#include "format.h"
#include "common.h"
#include "code.h"


/* Code */
/* private */
/* types */
typedef struct _CodeString
{
	int id;
	char * name;
	off_t offset;
	ssize_t size;
} CodeString;

struct _Code
{
	Arch * arch;
	ArchDescription * description;
	Format * format;
	char * filename;
	FILE * fp;

	/* strings */
	CodeString * strings;
	size_t strings_cnt;
};


/* prototypes */
/* strings */
static void _code_string_delete_all(Code * code);

static CodeString * _code_string_get_by_id(Code * code, AsmId id);
static int _code_string_set(CodeString * codestring, int id, char const * name,
		off_t offset, ssize_t size);

static CodeString * _code_string_append(Code * code);


/* functions */
/* code_new */
Code * code_new(char const * arch, char const * format)
{
	Code * code;

	if((code = object_new(sizeof(*code))) == NULL)
		return NULL;
	memset(code, 0, sizeof(*code));
	if((code->arch = arch_new(arch)) != NULL && format == NULL)
		format = arch_get_format(code->arch);
	if(format != NULL)
		code->format = format_new(format, arch);
	if(code->arch == NULL || code->format == NULL)
	{
		code_delete(code);
		return NULL;
	}
	code->description = arch_get_description(code->arch);
	return code;
}


/* code_delete */
int code_delete(Code * code)
{
	int ret = 0;

	if(code->format != NULL)
		format_delete(code->format);
	if(code->arch != NULL)
		arch_delete(code->arch);
	if(code->fp != NULL && fclose(code->fp) != 0)
		ret |= error_set_code(2, "%s: %s", code->filename, strerror(
					errno));
	string_delete(code->filename);
	object_delete(code);
	return ret;
}


/* accessors */
/* code_get_arch */
char const * code_get_arch(Code * code)
{
	return arch_get_name(code->arch);
}


/* code_get_filename */
char const * code_get_filename(Code * code)
{
	return code->filename;
}


/* code_get_format */
char const * code_get_format(Code * code)
{
	return format_get_name(code->format);
}


/* useful */
/* code_close */
int code_close(Code * code)
{
	int ret = 0;

	ret |= arch_exit(code->arch);
	ret |= format_exit(code->format);
	if(code->fp != NULL && fclose(code->fp) != 0 && ret == 0)
		ret |= -error_set_code(1, "%s: %s", code->filename,
				strerror(errno));
	code->fp = NULL;
	_code_string_delete_all(code);
	return ret;
}


/* code_decode */
int code_decode(Code * code, char const * buffer, size_t size)
{
	int ret;

	arch_init_buffer(code->arch, buffer, size);
	ret = arch_decode(code->arch);
	arch_exit(code->arch);
	return ret;
}


/* code_decode_file */
static int _decode_file_callback(void * priv, char const * section,
		off_t offset, size_t size, off_t base);
static int _set_string_callback(void * priv, int id, char const * name,
		off_t offset, ssize_t size);

int code_decode_file(Code * code, char const * filename)
{
	int ret;
	FILE * fp;

	if((fp = fopen(filename, "r")) == NULL)
		return -error_set_code(1, "%s: %s", filename, strerror(errno));
	arch_init(code->arch, filename, fp);
	format_init(code->format, filename, fp);
	ret = format_decode(code->format, _set_string_callback,
			_decode_file_callback, code);
	format_exit(code->format);
	arch_exit(code->arch);
	if(fclose(fp) != 0 && ret == 0)
		ret = -error_set_code(1, "%s: %s", filename, strerror(errno));
	return ret;
}

static int _decode_file_callback(void * priv, char const * section,
		off_t offset, size_t size, off_t base)
{
	Code * code = priv;

	if(section != NULL)
		printf("%s%s:\n", "\nDisassembly of section ", section);
	return arch_decode_at(code->arch, offset, size, base);
}

static int _set_string_callback(void * priv, int id, char const * name,
		off_t offset, ssize_t size)
{
	Code * code = priv;
	CodeString * cs = NULL;

	if(id >= 0)
		cs = _code_string_get_by_id(code, id);
	if(cs == NULL)
		cs = _code_string_append(code);
	if(cs == NULL || _code_string_set(cs, id, name, offset, size) != 0)
		return -1;
	return cs->id;
}


/* code_function */
int code_function(Code * code, char const * function)
{
	return format_function(code->format, function);
}


/* code_instruction */
int code_instruction(Code * code, ArchInstructionCall * call)
{
	ArchInstruction * ai;

	if((ai = arch_get_instruction_by_call(code->arch, call)) == NULL)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: instruction %s, opcode 0x%x, 1 0x%x, 2 0x%x"
			", 3 0x%x\n", call->name, ai->opcode, ai->op1, ai->op2,
			ai->op3);
#endif
	return arch_write(code->arch, ai, call);
}


/* code_open */
int code_open(Code * code, char const * filename)
{
	if(code->filename != NULL || code->fp != NULL)
		return -error_set_code(1, "A file is already opened");
	if((code->filename = string_new(filename)) == NULL)
		return -1;
	if((code->fp = fopen(filename, "w+")) == NULL)
		return -error_set_code(1, "%s: %s", filename, strerror(errno));
	if(format_init(code->format, code->filename, code->fp) != 0
			|| arch_init(code->arch, code->filename, code->fp) != 0)
	{
		fclose(code->fp);
		code->fp = NULL;
		unlink(code->filename); /* XXX may fail */
		string_delete(code->filename);
		code->filename = NULL;
		return -1;
	}
	return 0;
}


/* code_section */
int code_section(Code * code, char const * section)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, section);
#endif
	return format_section(code->format, section);
}


/* private */
/* functions */
/* strings */
/* code_string_delete_all */
static void _code_string_delete_all(Code * code)
{
	size_t i;

	for(i = 0; i < code->strings_cnt; i++)
		free(code->strings[i].name);
	code->strings_cnt = 0;
	free(code->strings);
	code->strings = NULL;
}


/* code_string_get_by_id */
static CodeString * _code_string_get_by_id(Code * code, AsmId id)
{
	size_t i;

	for(i = 0; i < code->strings_cnt; i++)
		if(code->strings[i].id >= 0 && code->strings[i].id == id)
			return &code->strings[i];
	return NULL;
}


/* code_string_set */
static int _code_string_set(CodeString * codestring, int id, char const * name,
		off_t offset, ssize_t size)
{
	char * p = NULL;

	if(name != NULL && (p = strdup(name)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	codestring->id = id;
	free(codestring->name);
	codestring->name = p;
	codestring->offset = offset;
	codestring->size = size;
	return 0;
}


/* code_string_append */
static CodeString * _code_string_append(Code * code)
{
	CodeString * p;

	if((p = realloc(code->strings, sizeof(*p) * (code->strings_cnt + 1)))
			== NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		return NULL;
	}
	code->strings = p;
	p = &code->strings[code->strings_cnt++];
	p->id = -1;
	p->name = NULL;
	p->offset = -1;
	p->size = -1;
	return p;
}
