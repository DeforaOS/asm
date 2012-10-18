/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel Asm */
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
/* TODO:
 * - lookup if a symbol is defined for each offset
 * - derive functions/labels/etc from symbols (type, id, name, union) */



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


/* AsmCode */
/* private */
/* types */
struct _AsmCode
{
	AsmArch * arch;
	AsmArchDescription * description;
	AsmFormat * format;
	char * filename;
	FILE * fp;

	/* elements */
	AsmElement * elements[AET_COUNT];
	size_t elements_cnt[AET_COUNT];
};


/* prototypes */
/* elements */
static void _asmcode_element_delete_all(AsmCode * code, AsmElementType type);

static AsmElement * _asmcode_element_get_by_id(AsmCode * code,
		AsmElementType type, AsmElementId id);
static int _asmcode_element_set(AsmElement * element, AsmElementId id,
		char const * name, off_t offset, ssize_t size, off_t base);

/* functions */
static void _asmcode_function_delete_all(AsmCode * code);
static AsmFunction * _asmcode_function_get_by_id(AsmCode * code,
		AsmFunctionId id);
static int _asmcode_function_set(AsmFunction * codefunction, AsmFunctionId id,
		char const * name, off_t offset, ssize_t size);

static AsmFunction * _asmcode_function_append(AsmCode * code);

/* sections */
static AsmSection * _asmcode_section_get_by_id(AsmCode * code, AsmSectionId id);
static int _asmcode_section_set(AsmSection * section, int id, char const * name,
		off_t offset, ssize_t size, off_t base);

static AsmSection * _asmcode_section_append(AsmCode * code);

/* strings */
static void _asmcode_string_delete_all(AsmCode * code);

static AsmString * _asmcode_string_get_by_id(AsmCode * code, AsmStringId id);
static int _asmcode_string_set(AsmCode * code, AsmString * codestring,
		int id, char const * name, off_t offset, ssize_t length);

static AsmString * _asmcode_string_append(AsmCode * code);
static int _asmcode_string_read(AsmCode * code, AsmString * codestring);


/* functions */
/* asmcode_new */
AsmCode * asmcode_new(char const * arch, char const * format)
{
	AsmCode * code;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", \"%s\")\n", __func__, arch, format);
#endif
	if((code = object_new(sizeof(*code))) == NULL)
		return NULL;
	memset(code, 0, sizeof(*code));
	code->arch = arch_new(arch);
	if(format != NULL)
		code->format = format_new(format);
	if(code->arch == NULL)
	{
		asmcode_delete(code);
		return NULL;
	}
	code->description = arch_get_description(code->arch);
	return code;
}


/* asmcode_new_file */
static AsmFormat * _new_file_format(char const * filename, FILE * fp);

AsmCode * asmcode_new_file(char const * arch, char const * format,
		char const * filename)
{
	AsmCode * code;
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
		asmcode_delete(code);
		return NULL;
	}
	code->description = arch_get_description(code->arch);
	return code;
}

static AsmFormat * _new_file_format(char const * filename, FILE * fp)
{
	char const path[] = LIBDIR "/" PACKAGE "/format";
	DIR * dir;
	struct dirent * de;
	size_t len;
#ifdef __APPLE__
	char const ext[] = ".dylib";
#else
	char const ext[] = ".so";
#endif
	int hasflat = 0;
	AsmFormat * format = NULL;

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
		if((len = strlen(de->d_name)) < sizeof(ext))
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


/* asmcode_delete */
int asmcode_delete(AsmCode * code)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
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
/* asmcode_get_arch */
char const * asmcode_get_arch(AsmCode * code)
{
	return arch_get_name(code->arch);
}


/* asmcode_get_arch_description */
AsmArchDescription * asmcode_get_arch_description(AsmCode * code)
{
	return arch_get_description(code->arch);
}


/* asmcode_get_filename */
char const * asmcode_get_filename(AsmCode * code)
{
	return code->filename;
}


/* asmcode_get_format */
char const * asmcode_get_format(AsmCode * code)
{
	return format_get_name(code->format);
}


/* asmcode_get_function_by_id */
AsmFunction * asmcode_get_function_by_id(AsmCode * code, AsmFunctionId id)
{
	return _asmcode_element_get_by_id(code, AET_FUNCTION, id);
}


/* asmcode_get_functions */
void asmcode_get_functions(AsmCode * code, AsmFunction ** functions,
		size_t * functions_cnt)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	*functions = code->elements[AET_FUNCTION];
	*functions_cnt = code->elements_cnt[AET_FUNCTION];
}


/* asmcode_get_section_by_id */
AsmSection * asmcode_get_section_by_id(AsmCode * code, AsmSectionId id)
{
	return _asmcode_element_get_by_id(code, AET_SECTION, id);
}


/* asmcode_get_sections */
void asmcode_get_sections(AsmCode * code, AsmSection ** sections,
		size_t * sections_cnt)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	*sections = code->elements[AET_SECTION];
	*sections_cnt = code->elements_cnt[AET_SECTION];
}


/* asmcode_get_string_by_id */
AsmString * asmcode_get_string_by_id(AsmCode * code, AsmStringId id)
{
	return _asmcode_element_get_by_id(code, AET_STRING, id);
}


/* asmcode_get_strings */
void asmcode_get_strings(AsmCode * code, AsmString ** strings,
		size_t * strings_cnt)
{
	*strings = code->elements[AET_STRING];
	*strings_cnt = code->elements_cnt[AET_STRING];
}


/* asmcode_set_function */
int asmcode_set_function(AsmCode * code, int id, char const * name,
		off_t offset, ssize_t size)
{
	AsmFunction * cf = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, \"%s\", %ld, %ld)\n", __func__, id, name,
			offset, size);
#endif
	if(id >= 0)
		cf = _asmcode_function_get_by_id(code, id);
	if(cf == NULL)
		cf = _asmcode_function_append(code);
	if(cf == NULL || _asmcode_function_set(cf, id, name, offset, size) != 0)
		return -1;
	/* FIXME isn't it considered an error if no ID is known yet? */
	return cf->id;
}


/* asmcode_set_section */
int asmcode_set_section(AsmCode * code, int id, char const * name, off_t offset,
		ssize_t size, off_t base)
{
	AsmSection * cs = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, \"%s\", %ld, %ld)\n", __func__, id, name,
			offset, size);
#endif
	if(id >= 0)
		cs = _asmcode_section_get_by_id(code, id);
	if(cs == NULL)
		cs = _asmcode_section_append(code);
	if(cs == NULL || _asmcode_section_set(cs, id, name, offset, size, base)
			!= 0)
		return -1;
	/* FIXME isn't it considered an error if no ID is known yet? */
	return cs->id;
}


/* asmcode_set_string */
int asmcode_set_string(AsmCode * code, int id, char const * name, off_t offset,
		ssize_t length)
{
	AsmString * cs = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(0x%x)\n", __func__, id);
#endif
	if(id >= 0)
		cs = _asmcode_string_get_by_id(code, id);
	if(cs == NULL)
		cs = _asmcode_string_append(code);
	if(cs == NULL || _asmcode_string_set(code, cs, id, name, offset,
				length) != 0)
		return -1;
	/* FIXME isn't it considered an error if no ID is known yet? */
	return cs->id;
}


/* useful */
/* asmcode_close */
int asmcode_close(AsmCode * code)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	ret |= arch_exit(code->arch);
	if(code->format != NULL)
		ret |= format_exit(code->format);
	if(code->fp != NULL && fclose(code->fp) != 0 && ret == 0)
		ret |= -error_set_code(1, "%s: %s", code->filename,
				strerror(errno));
	code->fp = NULL;
	_asmcode_string_delete_all(code);
	_asmcode_function_delete_all(code);
	return ret;
}


/* asmcode_decode */
int asmcode_decode(AsmCode * code, int raw)
{
	return format_decode(code->format, code, raw);
}


/* asmcode_decode_at */
int asmcode_decode_at(AsmCode * code, off_t offset, size_t size, off_t base,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%ld, %lu, %ld)\n", __func__, offset, size,
			base);
#endif
	if(arch_decode_at(code->arch, code, offset, size, base, calls,
				calls_cnt) != 0)
		return -1;
	return 0;
}


/* asmcode_decode_buffer */
int asmcode_decode_buffer(AsmCode * code, char const * buffer, size_t size,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	int ret;
	AsmArchDescription * description;

	arch_init_buffer(code->arch, buffer, size);
	description = arch_get_description(code->arch);
	ret = arch_decode(code->arch, code, 0, calls, calls_cnt);
	arch_exit(code->arch);
	return ret;
}


/* asmcode_decode_section */
int asmcode_decode_section(AsmCode * code, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	return format_decode_section(code->format, code, section, calls,
			calls_cnt);
}


/* asmcode_function */
int asmcode_function(AsmCode * code, char const * function)
{
	return format_function(code->format, function);
}


/* asmcode_instruction */
int asmcode_instruction(AsmCode * code, AsmArchInstructionCall * call)
{
	AsmArchInstruction * ai;

	if((ai = arch_get_instruction_by_call(code->arch, call)) == NULL)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: instruction %s, opcode 0x%x, 1 0x%x, 2 0x%x"
			", 3 0x%x\n", call->name, ai->opcode, ai->op1, ai->op2,
			ai->op3);
#endif
	return arch_encode(code->arch, ai, call);
}


/* asmcode_open */
int asmcode_open(AsmCode * code, char const * filename)
{
	if(code->filename != NULL || code->fp != NULL)
		return -error_set_code(1, "A file is already opened");
	if((code->filename = string_new(filename)) == NULL)
		return -1;
	if((code->fp = fopen(filename, "w+")) == NULL)
		return -error_set_code(1, "%s: %s", filename, strerror(errno));
	if(arch_init(code->arch, code->filename, code->fp) == 0)
	{
		if(code->format == NULL)
			code->format = format_new(arch_get_format(code->arch));
		if(code->format != NULL && format_init(code->format,
					arch_get_name(code->arch),
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


/* asmcode_print */
static void _print_address(AsmArchDescription * description,
		unsigned long address);
static void _print_immediate(AsmArchOperand * ao);

int asmcode_print(AsmCode * code, AsmArchInstructionCall * call)
{
	AsmArchDescription * description;
	char const * sep = " ";
	size_t i;
	uint8_t u8;
	AsmArchOperand * ao;
	char const * name;

	description = arch_get_description(code->arch);
	if(arch_seek(code->arch, call->offset, SEEK_SET) < 0)
		return -1;
	_print_address(description, call->base);
	for(i = 0; i < call->size; i++)
	{
		if(arch_read(code->arch, &u8, sizeof(u8)) != sizeof(u8))
			return -1;
		printf(" %02x", u8);
	}
	/* FIXME print on the following line if it was too long */
	for(; i < 8; i++)
		fputs("   ", stdout);
	if(call->operands_cnt == 0)
	{
		printf(" %s\n", call->name);
		return 0;
	}
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
				printf("[%%%s + $0x%lx]", name, (unsigned long)
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

static void _print_address(AsmArchDescription * description, unsigned long address)
{
	uint32_t size = (description != NULL) ? description->address_size : 32;
	char const * format = "%8lx:";

	switch(size)
	{
		case 64:
			format = "%16lx:";
			break;
		case 20:
			format = "%5lx:";
			break;
		default:
			break;
	}
	printf(format, address);
}

static void _print_immediate(AsmArchOperand * ao)
{
	printf("%s$0x%lx", ao->value.immediate.negative ? "-" : "",
			(unsigned long)ao->value.immediate.value);
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
			printf(" (call \"%s\")", ao->value.immediate.name);
		else
			printf("%s", " (call)");
	}
}


/* asmcode_section */
int asmcode_section(AsmCode * code, char const * section)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, section);
#endif
	return format_section(code->format, section);
}


/* private */
/* elements */
/* asmcode_element_set */
static int _asmcode_element_set(AsmElement * element, AsmElementId id,
		char const * name, off_t offset, ssize_t size, off_t base)
{
	char * p = NULL;

	if(name != NULL && (p = string_new(name)) == NULL)
		return -1;
	element->id = id;
	free(element->name);
	element->name = p;
	element->offset = offset;
	element->size = size;
	element->base = base;
	return 0;
}


static AsmElement * _asmcode_element_append(AsmCode * code, AsmElementType type)
{
	AsmElement * p = code->elements[type];
	size_t cnt = code->elements_cnt[type];

	if((p = realloc(p, sizeof(*p) * (cnt + 1))) == NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		return NULL;
	}
	code->elements[type] = p;
	p = &code->elements[type][cnt];
	code->elements_cnt[type]++;
	p->id = -1;
	p->name = NULL;
	p->offset = -1;
	p->size = -1;
	return p;
}


static void _asmcode_element_delete_all(AsmCode * code, AsmElementType type)
{
	size_t i;

	for(i = 0; i < code->elements_cnt[type]; i++)
		free(code->elements[type][i].name);
	code->elements_cnt[type] = 0;
	free(code->elements[type]);
	code->elements[type] = NULL;
}


static AsmElement * _asmcode_element_get_by_id(AsmCode * code,
		AsmElementType type, AsmElementId id)
{
	size_t i;

	for(i = 0; i < code->elements_cnt[type]; i++)
		if(code->elements[type][i].id >= 0
				&& code->elements[type][i].id == id)
			return &code->elements[type][i];
	return NULL;
}


/* functions */
/* asmcode_function_delete_all */
static void _asmcode_function_delete_all(AsmCode * code)
{
	_asmcode_element_delete_all(code, AET_FUNCTION);
}


/* asmcode_function_get_by_id */
static AsmFunction * _asmcode_function_get_by_id(AsmCode * code,
		AsmFunctionId id)
{
	return _asmcode_element_get_by_id(code, AET_FUNCTION, id);
}


/* asmcode_function_set */
static int _asmcode_function_set(AsmFunction * codefunction, AsmFunctionId id,
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


/* asmcode_function_append */
static AsmFunction * _asmcode_function_append(AsmCode * code)
{
	return _asmcode_element_append(code, AET_FUNCTION);
}


/* sections */
/* asmcode_section_get_by_id */
static AsmSection * _asmcode_section_get_by_id(AsmCode * code, AsmSectionId id)
{
	return _asmcode_element_get_by_id(code, AET_SECTION, id);
}


/* asmcode_section_set */
static int _asmcode_section_set(AsmSection * section, int id, char const * name,
		off_t offset, ssize_t size, off_t base)
{
	return _asmcode_element_set(section, id, name, offset, size, base);
}


/* asmcode_section_append */
static AsmSection * _asmcode_section_append(AsmCode * code)
{
	return _asmcode_element_append(code, AET_SECTION);
}


/* strings */
/* asmcode_string_delete_all */
static void _asmcode_string_delete_all(AsmCode * code)
{
	_asmcode_element_delete_all(code, AET_STRING);
}


/* asmcode_string_get_by_id */
static AsmString * _asmcode_string_get_by_id(AsmCode * code, AsmStringId id)
{
	AsmString * ret;

	if((ret = _asmcode_element_get_by_id(code, AET_STRING, id)) == NULL)
		return NULL;
	if(ret->name == NULL && ret->size > 0)
		_asmcode_string_read(code, ret);
	return ret;
}


/* asmcode_string_set */
static int _asmcode_string_set(AsmCode * code, AsmString * codestring, int id,
		char const * name, off_t offset, ssize_t length)
{
	if(_asmcode_element_set(codestring, id, name, offset, length, 0) != 0)
		return -1;
	if(name == NULL && length > 0)
		_asmcode_string_read(code, codestring);
	return 0;
}


/* asmcode_string_append */
static AsmString * _asmcode_string_append(AsmCode * code)
{
	return _asmcode_element_append(code, AET_STRING);
}


/* asmcode_string_read */
static int _asmcode_string_read(AsmCode * code, AsmString * codestring)
{
	off_t offset; /* XXX should not have to be kept */
	char * buf;

	/* FIXME if offset < 0 read until '\0' */
	if(codestring->offset < 0 || codestring->size < 0)
		return -error_set_code(1, "%s", "Insufficient information to"
				" read string");
	if((offset = arch_seek(code->arch, 0, SEEK_CUR)) < 0)
		return -1;
	if((buf = malloc(codestring->size + 1)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	if(arch_seek(code->arch, codestring->offset, SEEK_SET)
			!= codestring->offset)
		return -1;
	if(arch_read(code->arch, buf, codestring->size) != codestring->size)
	{
		free(buf);
		arch_seek(code->arch, offset, SEEK_SET);
		return -1;
	}
	buf[codestring->size] = '\0';
	free(codestring->name);
	codestring->name = buf;
	return arch_seek(code->arch, offset, SEEK_SET);
}
