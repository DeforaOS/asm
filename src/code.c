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
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "arch.h"
#include "format.h"
#include "code.h"
#include "../config.h"


/* Code */
/* private */
/* types */
typedef struct _CodeString
{
	int id;
	char * name;
	off_t offset;
	ssize_t length;
} CodeString;

typedef struct _CodeFunction
{
	int id;
	char * name;
	off_t offset;
	ssize_t size;
} CodeFunction;

struct _Code
{
	Arch * arch;
	ArchDescription * description;
	Format * format;
	char * filename;
	FILE * fp;

	/* functions */
	CodeFunction * functions;
	size_t functions_cnt;

	/* strings */
	CodeString * strings;
	size_t strings_cnt;
};


/* prototypes */
/* functions */
static void _code_function_delete_all(Code * code);

static CodeFunction * _code_function_get_by_id(Code * code, AsmId id);
static int _code_function_set(CodeFunction * codefunction, int id,
		char const * name, off_t offset, ssize_t size);

static CodeFunction * _code_function_append(Code * code);

/* strings */
static void _code_string_delete_all(Code * code);

static CodeString * _code_string_get_by_id(Code * code, AsmId id);
static int _code_string_set(CodeString * codestring, int id, char const * name,
		off_t offset, ssize_t length);

static CodeString * _code_string_append(Code * code);
static int _code_string_read(Code * code, CodeString * codestring);


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
		code->format = format_new(format);
	if(code->arch == NULL || code->format == NULL)
	{
		code_delete(code);
		return NULL;
	}
	code->description = arch_get_description(code->arch);
	return code;
}


/* code_new_file */
static Format * _new_file_format(char const * filename, FILE * fp);

Code * code_new_file(char const * arch, char const * format,
		char const * filename)
{
	Code * code;
	FILE * fp;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, filename);
#endif
	if((fp = fopen(filename, "r")) == NULL)
	{
		error_set_code(1, "%s: %s", filename, strerror(errno));
		return NULL;
	}
	if((code = object_new(sizeof(*code))) == NULL)
	{
		fclose(fp);
		return NULL;
	}
	memset(code, 0, sizeof(*code));
	code->filename = string_new(filename);
	if(format == NULL)
		code->format = _new_file_format(filename, fp);
	else if((code->format = format_new(format)) != NULL
			&& format_init(code->format, NULL, filename, fp) != 0)
	{
		format_delete(code->format);
		code->format = NULL;
	}
	if(arch == NULL && code->format != NULL)
		arch = format_detect_arch(code->format);
	if(arch != NULL && (code->arch = arch_new(arch)) != NULL
			&& arch_init(code->arch, filename, fp) != 0)
	{
		arch_delete(code->arch);
		code->arch = NULL;
	}
	if(code->filename == NULL || code->arch == NULL || code->format == NULL)
	{
		code_delete(code);
		return NULL;
	}
	code->description = arch_get_description(code->arch);
	return code;
}

static Format * _new_file_format(char const * filename, FILE * fp)
{
	char const path[] = LIBDIR "/" PACKAGE "/format";
	DIR * dir;
	struct dirent * de;
	size_t len;
	char const ext[] = ".so";
	int hasflat = 0;
	Format * format = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, filename);
#endif
	if((dir = opendir(path)) == NULL)
	{
		error_set_code(1, "%s: %s", path, strerror(errno));
		return NULL;
	}
	while((de = readdir(dir)) != NULL)
	{
		if((len = strlen(de->d_name)) < 4)
			continue;
		if(strcmp(&de->d_name[len - sizeof(ext) + 1], ext) != 0)
			continue;
		de->d_name[len - sizeof(ext) + 1] = '\0';
		if(strcmp(de->d_name, "flat") == 0)
			hasflat = 1;
		if((format = format_new(de->d_name)) == NULL)
			continue;
		if(format_init(format, NULL, filename, fp) == 0
				&& format_match(format) == 1)
			break;
		format_delete(format);
		format = NULL;
	}
	closedir(dir);
	/* fallback on the "flat" format plug-in if necessary and available */
	if(format == NULL && hasflat && (format = format_new("flat")) != NULL
			&& format_init(format, NULL, filename, fp) != 0)
		{
			format_delete(format);
			format = NULL;
		}
	return format;
}


/* code_delete */
int code_delete(Code * code)
{
	int ret = 0;

	_code_function_delete_all(code);
	_code_string_delete_all(code);
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


/* code_get_function_by_id */
AsmFunction * code_get_function_by_id(Code * code, AsmId id)
{
	/* XXX CodeFunction has to be exactly like an AsmFunction */
	return _code_function_get_by_id(code, id);
}


/* code_get_string_by_id */
AsmString * code_get_string_by_id(Code * code, AsmId id)
{
	/* XXX CodeString has to be exactly like an AsmString */
	return _code_string_get_by_id(code, id);
}


/* code_set_function */
int code_set_function(Code * code, int id, char const * name, off_t offset,
		ssize_t size)
{
	CodeFunction * cf = NULL;

	if(id >= 0)
		cf = _code_function_get_by_id(code, id);
	if(cf == NULL)
		cf = _code_function_append(code);
	if(cf == NULL || _code_function_set(cf, id, name, offset, size) != 0)
		return -1;
	/* FIXME isn't it considered an error if no ID is known yet? */
	return cf->id;
}


/* code_set_string */
int code_set_string(Code * code, int id, char const * name, off_t offset,
		ssize_t length)
{
	CodeString * cs = NULL;

	if(id >= 0)
		cs = _code_string_get_by_id(code, id);
	if(cs == NULL)
		cs = _code_string_append(code);
	if(cs == NULL || _code_string_set(cs, id, name, offset, length) != 0)
		return -1;
	/* FIXME isn't it considered an error if no ID is known yet? */
	return cs->id;
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
int code_decode(Code * code)
{
	printf("%s: %s-%s\n", code->filename, format_get_name(code->format),
			arch_get_name(code->arch));
	return format_decode(code->format, code);
}


/* code_decode_at */
int code_decode_at(Code * code, char const * section, off_t offset,
		size_t size, off_t base)
{
	ArchInstructionCall * calls = NULL;
	size_t calls_cnt = 0;
	size_t i;

	if(section != NULL)
		printf("%s%s:\n", "\nDisassembly of section ", section);
	if(arch_decode_at(code->arch, code, &calls, &calls_cnt, offset, size,
				base) != 0)
		return -1;
	if(size != 0)
		printf("\n%08lx:\n", (long)offset + (long)base);
	for(i = 0; i < calls_cnt; i++)
		code_print(code, &calls[i]);
	free(calls);
	return 0;
}


/* code_decode_buffer */
int code_decode_buffer(Code * code, char const * buffer, size_t size)
{
	int ret;
	ArchInstructionCall * calls = NULL;
	size_t calls_cnt = 0;
	size_t i;

	arch_init_buffer(code->arch, buffer, size);
	if((ret = arch_decode(code->arch, code, &calls, &calls_cnt)) == 0)
	{
		fprintf(stderr, "DEBUG: %lu\n", calls_cnt);
		for(i = 0; i < calls_cnt; i++)
			code_print(code, &calls[i]);
		free(calls);
	}
	arch_exit(code->arch);
	return ret;
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
	if(arch_init(code->arch, code->filename, code->fp) == 0)
	{
		if(format_init(code->format, arch_get_name(code->arch),
					code->filename, code->fp) == 0)
			return 0;
		arch_exit(code->arch);
	}
	fclose(code->fp);
	code->fp = NULL;
	unlink(code->filename); /* XXX may fail */
	string_delete(code->filename);
	code->filename = NULL;
	return -1;
}


/* code_print */
static void _print_immediate(ArchOperand * ao);

int code_print(Code * code, ArchInstructionCall * call)
{
	char const * sep = " ";
	size_t i;
	uint8_t u8;
	ArchOperand * ao;
	char const * name;

	if(arch_seek(code->arch, call->offset, SEEK_SET) < 0)
		return -1;
	printf("%8lx:", (long)call->base + (long)call->offset);
	for(i = 0; i < call->size; i++)
	{
		if(arch_read(code->arch, &u8, sizeof(u8)) != sizeof(u8))
			return -1;
		printf(" %02x", u8);
	}
	for(; i < 8; i++)
		printf("   ");
	printf(" %-12s", call->name);
	for(i = 0; i < call->operands_cnt; i++)
	{
		ao = &call->operands[i];
		fputs(sep, stdout);
		switch(AO_GET_TYPE(ao->definition))
		{
			case AOT_DREGISTER:
				name = ao->value.dregister.name;
				if(ao->value.dregister.offset == 0)
				{
					printf("[%%%s]", name);
					break;
				}
				printf("[%%%s + $0x%lx]", name,
						ao->value.dregister.offset);
				break;
			case AOT_DREGISTER2:
				name = ao->value.dregister2.name;
				printf("[%%%s + %%%s]", name,
						ao->value.dregister2.name2);
				break;
			case AOT_IMMEDIATE:
				_print_immediate(ao);
				break;
			case AOT_REGISTER:
				name = call->operands[i].value._register.name;
				printf("%%%s", name);
				break;
		}
		sep = ", ";
	}
	putchar('\n');
	return 0;
}

static void _print_immediate(ArchOperand * ao)
{
	printf("%s$0x%lx", ao->value.immediate.negative ? "-" : "",
			ao->value.immediate.value);
	if(AO_GET_VALUE(ao->definition) == AOI_REFERS_STRING)
	{
		if(ao->value.immediate.name != NULL)
			printf(" \"%s\"", ao->value.immediate.name);
		else
			printf("%s", " (string)");
	}
	else if(AO_GET_VALUE(ao->definition) == AOI_REFERS_FUNCTION)
	{
		if(ao->value.immediate.name != NULL)
			printf(" call \"%s\"", ao->value.immediate.name);
		else
			printf("%s", " (call)");
	}
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
/* functions */
/* code_function_delete_all */
static void _code_function_delete_all(Code * code)
{
	size_t i;

	for(i = 0; i < code->functions_cnt; i++)
		free(code->functions[i].name);
	code->functions_cnt = 0;
	free(code->functions);
	code->functions = NULL;
}


/* code_function_get_by_id */
static CodeFunction * _code_function_get_by_id(Code * code, AsmId id)
{
	size_t i;

	for(i = 0; i < code->functions_cnt; i++)
		if(code->functions[i].id >= 0
				&& (AsmId)code->functions[i].id == id)
			break;
	if(i == code->functions_cnt)
		return NULL;
	return &code->functions[i];
}


/* code_function_set */
static int _code_function_set(CodeFunction * codefunction, int id,
		char const * name, off_t offset, ssize_t size)
{
	char * p = NULL;

	if(name != NULL && (p = string_new(name)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	codefunction->id = id;
	free(codefunction->name);
	codefunction->name = p;
	codefunction->offset = offset;
	codefunction->size = size;
	return 0;
}


/* code_function_append */
static CodeFunction * _code_function_append(Code * code)
{
	CodeFunction * p;

	if((p = realloc(code->functions, sizeof(*p) * (code->functions_cnt
						+ 1))) == NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		return NULL;
	}
	code->functions = p;
	p = &code->functions[code->functions_cnt++];
	p->id = -1;
	p->name = NULL;
	p->offset = -1;
	p->size = -1;
	return p;
}


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
		if(code->strings[i].id >= 0 && (AsmId)code->strings[i].id == id)
			break;
	if(i == code->strings_cnt)
		return NULL;
	if(code->strings[i].name == NULL)
		_code_string_read(code, &code->strings[i]);
	return &code->strings[i];
}


/* code_string_set */
static int _code_string_set(CodeString * codestring, int id, char const * name,
		off_t offset, ssize_t length)
{
	char * p = NULL;

	if(name != NULL && (p = string_new(name)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	codestring->id = id;
	free(codestring->name);
	codestring->name = p;
	codestring->offset = offset;
	codestring->length = length;
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
	p->length = -1;
	return p;
}


/* code_string_read */
static int _code_string_read(Code * code, CodeString * codestring)
{
	off_t offset; /* XXX should not have to be kept */
	char * buf;

	if(codestring->offset < 0 || codestring->length < 0)
		return -error_set_code(1, "%s", "Insufficient information to"
				" read string");
	if((offset = arch_seek(code->arch, 0, SEEK_CUR)) < 0)
		return -1;
	if((buf = malloc(codestring->length + 1)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	if(arch_seek(code->arch, codestring->offset, SEEK_SET)
			!= codestring->offset)
		return -1;
	if(arch_read(code->arch, buf, codestring->length) != codestring->length)
	{
		free(buf);
		arch_seek(code->arch, offset, SEEK_SET);
		return -1;
	}
	buf[codestring->length] = '\0';
	free(codestring->name);
	codestring->name = buf;
	arch_seek(code->arch, offset, SEEK_SET);
	return 0;
}
