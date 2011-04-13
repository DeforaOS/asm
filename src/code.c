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
struct _Code
{
	Arch * arch;
	ArchDescription * description;
	Format * format;
	char * filename;
	FILE * fp;
};


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
Arch * code_get_arch(Code * code)
{
	return code->arch;
}


/* code_get_arch_name */
char const * code_get_arch_name(Code * code)
{
	return arch_get_name(code->arch);
}


/* code_get_format */
Format * code_get_format(Code * code)
{
	return code->format;
}


/* code_get_format_name */
char const * code_get_format_name(Code * code)
{
	return format_get_name(code->format);
}


/* useful */
/* code_close */
int code_close(Code * code)
{
	int ret;

	ret = format_exit(code->format);
	if(fclose(code->fp) != 0)
		ret |= -error_set_code(1, "%s: %s", code->filename,
				strerror(errno));
	return ret;
}


/* code_decode */
static ArchInstruction * _decode_size(Code * code, size_t * size,
		ArchInstruction * ai);

ArchInstruction * code_decode(Code * code, char const * buffer, size_t * size)
{
	size_t i;
	uint32_t opcode = 0;
	ArchInstruction * ai;

	if(size == NULL || *size == 0)
		return NULL;
	for(i = 0; i < *size && i < sizeof(opcode); i++)
	{
		opcode = (opcode << 8) | (unsigned char)buffer[i];
		if((ai = arch_get_instruction_by_opcode(code->arch, i + 1,
						opcode)) != NULL)
			return _decode_size(code, size, ai);
	}
	if((ai = arch_get_instruction_by_name(code->arch, "db")) != NULL)
		return _decode_size(code, size, ai);
	return NULL;
}

static ArchInstruction * _decode_size(Code * code, size_t * size,
		ArchInstruction * ai)
{
	size_t s;

	if((s = code->description->instruction_size) == 0)
	{
		s = AO_GET_SIZE(ai->opcode);
		s += AO_GET_SIZE(ai->op1);
		s += AO_GET_SIZE(ai->op2);
		s += AO_GET_SIZE(ai->op3);
	}
	if(s > *size)
		return NULL;
	*size = s;
	return ai;
}


/* code_function */
int code_function(Code * code, char const * function)
{
	return format_function(code->format, function);
}


/* code_instruction */
static int _instruction_fixed(Code * code, ArchInstruction * ai,
		AsOperand ** operands, size_t operands_cnt);
static int _instruction_fixed_immediate(ArchOperand operand, AsOperand * aso,
		uint32_t * pu);
static int _instruction_fixed_register(Code * code, ArchOperand operand,
		AsOperand * aso, uint32_t * pu);
static int _instruction_variable(Code * code, ArchInstruction * ai,
		AsOperand ** operands, size_t operands_cnt);
static int _instruction_variable_dregister(Code * code, ArchOperand operand,
		char const * name);
static int _instruction_variable_immediate(Code * code, ArchOperand operand,
		void * value, int swap);
static int _instruction_variable_opcode(Code * code, ArchInstruction * ai);
static int _instruction_variable_operand(Code * code, ArchOperand operand,
		AsOperand * aso);
static int _instruction_variable_register(Code * code, ArchOperand operand,
		char const * name);

int code_instruction(Code * code, char const * name, AsOperand ** operands,
		size_t operands_cnt)
{
	ArchInstruction * ai;

	if((ai = arch_get_instruction_by_operands(code->arch, name, operands,
					operands_cnt)) == NULL)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: instruction %s, opcode 0x%x, 1 0x%x, 2 0x%x"
			", 3 0x%x\n", name, ai->value, ai->op1, ai->op2,
			ai->op3);
#endif
	if(code->description != NULL && code->description->instruction_size)
		return _instruction_fixed(code, ai, operands, operands_cnt);
	return _instruction_variable(code, ai, operands, operands_cnt);
}

static int _instruction_fixed(Code * code, ArchInstruction * ai,
		AsOperand ** operands, size_t operands_cnt)
{
	uint32_t instruction;
	size_t i;
	uint32_t u;
	ArchOperand operand;

	instruction = ai->value;
	for(i = 0, u = 0; i < operands_cnt; i++, u = 0)
	{
		operand = (i == 0) ? ai->op1 : ((i == 1) ? ai->op2 : ai->op3);
		switch(AO_GET_TYPE(operand))
		{
			case AOT_DREGISTER:
			case AOT_REGISTER:
				if(_instruction_fixed_register(code, operand,
							operands[i], &u) != 0)
					return -1;
				break;
			case AOT_IMMEDIATE:
				if(_instruction_fixed_immediate(operand,
							operands[i], &u) != 0)
					return -1;
				break;
		}
		instruction |= u;
	}
	/* FIXME check if it is always the case */
	instruction = htonl(instruction);
	if(fwrite(&instruction, sizeof(instruction), 1, code->fp) != 1)
		return -error_set_code(1, "%s: %s", code->filename,
				strerror(errno));
	return 0;
}

static int _instruction_fixed_immediate(ArchOperand operand, AsOperand * aso,
		uint32_t * pu)
{
	long l;
	unsigned long u;
	uint32_t size;

	if(aso->value == NULL)
		return -1; /* XXX report error */
	if(AO_GET_FLAGS(operand) & AOF_SIGNED)
	{
		l = *(long*)aso->value;
		u = l;
	}
	else
		u = *(unsigned long*)aso->value;
	if((size = AO_GET_SIZE(operand)) > 0)
	{
		size = (1 << (size + 1)) - 1;
		u &= size;
	}
	if(AO_GET_FLAGS(operand) & AOF_SOFFSET)
		u >>= AO_GET_OFFSET(operand);
	else
		u <<= AO_GET_OFFSET(operand);
	*pu = u;
	return 0;
}

static int _instruction_fixed_register(Code * code, ArchOperand operand,
		AsOperand * aso, uint32_t * pu)
{
	ArchRegister * ar;

	if((ar = arch_get_register_by_name(code->arch, aso->value)) == NULL)
		return -1;
	*pu = ar->id;
	if(AO_GET_FLAGS(operand) & AOF_SOFFSET)
		*pu >>= AO_GET_OFFSET(operand);
	else
		*pu <<= AO_GET_OFFSET(operand);
	return 0;
}

static int _instruction_variable(Code * code, ArchInstruction * ai,
		AsOperand ** operands, size_t operands_cnt)
{
	size_t i;
	ArchOperand operand;

	if(_instruction_variable_opcode(code, ai) != 0)
		return -1;
	for(i = 0; i < operands_cnt; i++)
	{
		if(i == 0)
			operand = ai->op1;
		else if(i == 1)
			operand = ai->op2;
		else if(i == 2)
			operand = ai->op3;
		else
			return -1; /* XXX report error */
		if(_instruction_variable_operand(code, operand, operands[i])
				!= 0)
			return -1;
	}
	return 0;
}

static int _instruction_variable_dregister(Code * code, ArchOperand operand,
		char const * name)
{
	ArchRegister * ar;
	uint32_t value;
	uint32_t offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	if((ar = arch_get_register_by_name(code->arch, name)) == NULL)
		return -1;
	value = ar->id;
	if(AO_GET_FLAGS(operand) & AOF_OFFSETSIZE)
	{
		offset = AO_GET_OFFSET(operand);
		operand &= ~(AOM_OFFSET | AOM_SIZE);
		operand |= (offset << AOD_SIZE);
	}
	else
		value <<= AO_GET_OFFSET(operand);
	return _instruction_variable_immediate(code, operand, &value, 0);
}

static int _instruction_variable_immediate(Code * code, ArchOperand operand,
		void * value, int swap)
{
	size_t size;
	void * buf;
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%d)\n", __func__, swap);
#endif
	if((size = AO_GET_SIZE(operand)) == 0)
		return -error_set_code(1, "%s", "Empty immediate value");
	else if(size <= 8)
	{
		u8 = *(uint8_t*)value;
		buf = &u8;
		size = 1;
	}
	else if(size <= 16)
	{
		u16 = *(uint16_t*)value;
		if(swap)
			u16 = htons(u16);
		buf = &u16;
		size = 2;
	}
	else if(size <= 24) /* FIXME merge with 32? */
	{
		u32 = *(uint32_t*)value;
		if(swap)
			u32 = htonl(u32 << 8);
		else
			u32 <<= 8;
		buf = &u32;
		size = 3;
	}
	else if(size <= 32)
	{
		u32 = *(uint32_t*)value;
		if(swap)
			u32 = htonl(u32);
		buf = &u32;
		size = 4;
	}
	else
		return -error_set_code(1, "%u: Size not implemented", size);
	if(fwrite(buf, size, 1, code->fp) != 1)
		return -error_set_code(1, "%s: %s", code->filename, strerror(
					errno));
	return 0;
}

static int _instruction_variable_opcode(Code * code, ArchInstruction * ai)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() 0x%x\n", __func__, ai->value);
#endif
	return _instruction_variable_immediate(code, ai->opcode, &ai->value, 1);
}

static int _instruction_variable_operand(Code * code, ArchOperand operand,
		AsOperand * aso)
{
	switch(AO_GET_TYPE(operand))
	{
		case AOT_IMMEDIATE:
			return _instruction_variable_immediate(code, operand,
					aso->value, 1);
		case AOT_DREGISTER:
			return _instruction_variable_dregister(code, operand,
					aso->value);
		case AOT_REGISTER:
			return _instruction_variable_register(code, operand,
					aso->value);
		default:
			/* FIXME implement */
			return -error_set_code(1, "%s", strerror(ENOSYS));
	}
	return 0;
}

static int _instruction_variable_register(Code * code, ArchOperand operand,
		char const * name)
{
	ArchRegister * ar;
	uint32_t value;
	uint32_t offset;

	/* FIXME consider merging with _instruction_variable_dregister() */
	if(AO_GET_FLAGS(operand) & AOF_IMPLICIT)
		return 0;
	if((ar = arch_get_register_by_name(code->arch, name)) == NULL)
		return -1;
	/* FIXME really implement */
	value = ar->id;
	if(AO_GET_FLAGS(operand) & AOF_OFFSETSIZE)
	{
		offset = AO_GET_OFFSET(operand);
		operand &= ~(AOM_OFFSET | AOM_SIZE);
		operand |= (offset << AOD_SIZE);
	}
	else
		value <<= AO_GET_OFFSET(operand);
	return _instruction_variable_immediate(code, operand, &value, 0);
}
#if 0
	switch(AO_GET_SIZE(ai->opcode))
	{
		case sizeof(u8):
			u8 = ai->opcode;
			buf = &u8;
			break;
		case sizeof(u16):
			u16 = htons(ai->opcode);
			buf = &u16;
			break;
		case sizeof(u32):
		default:
			if(AO_GET_SIZE(ai->opcode) == 3) /* FIXME will break */
				u32 = htonl(ai->opcode << 8);
			else
				u32 = htonl(ai->opcode);
			buf = &u32;
			break;
	}
	if(AO_GET_SIZE(ai->opcode) != 0 && fwrite(buf, AO_GET_SIZE(ai->opcode),
				1, code->fp) != 1)
		return -error_set_code(1, "%s: %s", code->filename, strerror(
					errno));
	for(i = 0; i < operands_cnt; i++)
	{
		if(i >= 3)
			return -error_set_code(1, "%s: %s", name,
					"Too many arguments");
		size = AO_GET_SIZE(operands[i]->operand);
		if(size == 0)
			continue;
		u = *(unsigned long*)operands[i]->value;
		switch(AO_GET_TYPE(operands[i]->operand))
		{
			case AOT_IMMEDIATE:
				/* FIXME there still is an endian problem */
				switch(size)
				{
					case 1:
						u8 = u;
						buf = &u8;
						break;
					case 2:
						u16 = u;
						buf = &u16;
						break;
					default: /* FIXME not always so */
					case 4:
						buf = &u;
						break;
				}
				break;
			case AOT_REGISTER:
			default:
				/* FIXME really implement */
				buf = NULL;
				break;
		}
		if(buf != NULL && fwrite(buf, size, 1, code->fp) != 1)
			return -error_set_code(1, "%s: %s", code->filename,
					strerror(errno));
	}
	return 0;
}

#endif
#if 0
static int _instruction_instruction(Code * code, ArchInstruction ** ai,
		char const * instruction, AsOperand * operands[],
		size_t operands_cnt)
{
	size_t i;
	int cmp;
	int found = 0;

	/* FIXME check */
	for(i = 0; ((*ai) = arch_get_instruction(code->arch, i)) != NULL; i++)
	{
		/* FIXME alphabetical order assumption disabled for 80x86 */
		if((cmp = strcmp(instruction, (*ai)->name)) != 0)
			continue;
		found = 1;
		if(_instruction_operands(code, *ai, operands, operands_cnt)
				!= 0)
			continue;
		return 0;
	}
	return error_set_code(1, "%s \"%s\"", found ? "Invalid arguments to"
		: "Unknown instruction", instruction);
}

static int _instruction_operands(Code * code, ArchInstruction * ai,
		AsOperand * operands[], size_t operands_cnt)
{
	unsigned long op = 0;
	unsigned long o;
	char const * reg;
	size_t i;
	ArchRegister * ar;

	for(i = 0; i < operands_cnt; i++)
	{
		switch(operands[i]->type)
		{
			case AOT_IMMEDIATE:
				/* FIXME also check the operand size */
				o = _AO_IMM;
#ifdef DEBUG
				fprintf(stderr, "DEBUG: op %lu: imm; ", i);
#endif
				break;
			case AOT_REGISTER:
#if 0 /* XXX this looked maybe better */
				reg = operands[i].value + 1; /* "%rg" => "rg" */
#else
				reg = operands[i]->value;
#endif
				if((ar = _operands_register(code->arch, reg))
						== NULL)
					return 1;
				if(operands[i]->dereference)
					o = (_AO_DREG | (ar->id << 2));
				else
					o = (_AO_REG | (ar->id << 2));
#ifdef DEBUG
				fprintf(stderr, "DEBUG: op %lu: reg %s; ", i,
						reg);
#endif
				break;
			default:
				o = 0;
				break;
		}
		op |= o << (i * 8);
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: 0x%lx & 0x%x => 0x%lx\n", op, ai->operands,
			op & ai->operands);
#endif
	return (op == ai->operands) ? 0 : 1;
}

static ArchRegister * _operands_register(Arch * arch, char const * name)
{
	ArchRegister * ret;
	size_t i;

	for(i = 0; (ret = arch_register_get(arch, i)) != NULL; i++)
		if(strcmp(ret->name, name) == 0)
			break;
	return ret;
}
#endif


/* code_open */
int code_open(Code * code, char const * filename)
{
	if(code->filename != NULL || code->fp != NULL)
		return -error_set_code(1, "A file is already opened");
	if((code->filename = string_new(filename)) == NULL)
		return -1;
	if((code->fp = fopen(filename, "w+")) == NULL)
		return -error_set_code(1, "%s: %s", filename, strerror(errno));
	if(format_init(code->format, code->filename, code->fp) != 0)
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
