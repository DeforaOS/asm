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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm/arch.h"
#include "Asm/asm.h"
#include "arch.h"
#include "../config.h"

/* macros */
#ifndef abs
# define abs(a)		((a) >= 0 ? (a) : -(a))
#endif
#ifndef min
# define min(a, b)	((a) < (b) ? (a) : (b))
#endif


/* Arch */
/* private */
/* types */
struct _Arch
{
	ArchPluginHelper helper;
	Plugin * handle;
	ArchPlugin * plugin;
	size_t instructions_cnt;
	size_t registers_cnt;

	/* internal */
	char const * filename;
	FILE * fp;
	char const * buffer;
	size_t buffer_cnt;
	size_t buffer_pos;
};


/* macros */
#define AI_GET_OPERAND_COUNT(ai) (((ai->op1 & AOM_TYPE) ? 1 : 0) + \
		((ai->op2 & AOM_TYPE) ? 1 : 0) + \
		((ai->op3 & AOM_TYPE) ? 1 : 0))


/* prototypes */
/* callbacks */
static char const * _arch_get_filename(Arch * arch);
static ssize_t _arch_read(Arch * arch, void * buf, size_t size);
static ssize_t _arch_read_buffer(Arch * arch, void * buf, size_t size);
static ssize_t _arch_write(Arch * arch, void const * buf, size_t size);


/* public */
/* functions */
/* arch_new */
Arch * arch_new(char const * name)
{
	Arch * a;
	Plugin * handle;
	ArchPlugin * plugin;

	if((handle = plugin_new(LIBDIR, PACKAGE, "arch", name)) == NULL)
		return NULL;
	if((plugin = plugin_lookup(handle, "arch_plugin")) == NULL)
	{
		plugin_delete(handle);
		return NULL;
	}
	if((a = object_new(sizeof(*a))) == NULL)
	{
		object_delete(a);
		plugin_delete(handle);
		return NULL;
	}
	memset(&a->helper, 0, sizeof(a->helper));
	a->handle = handle;
	a->plugin = plugin;
	a->instructions_cnt = 0;
	if(a->plugin->instructions != NULL)
		for(; a->plugin->instructions[a->instructions_cnt].name != NULL;
				a->instructions_cnt++);
	a->registers_cnt = 0;
	if(a->plugin->registers != NULL)
		for(; a->plugin->registers[a->registers_cnt].name != NULL;
				a->registers_cnt++);
	a->filename = NULL;
	a->fp = NULL;
	a->buffer = NULL;
	a->buffer_cnt = 0;
	a->buffer_pos = 0;
	return a;
}


/* arch_delete */
void arch_delete(Arch * arch)
{
	plugin_delete(arch->handle);
	object_delete(arch);
}


/* accessors */
/* arch_get_description */
ArchDescription * arch_get_description(Arch * arch)
{
	return arch->plugin->description;
}


/* arch_get_format */
char const * arch_get_format(Arch * arch)
{
	if(arch->plugin->description != NULL
			&& arch->plugin->description->format != NULL)
		return arch->plugin->description->format;
	return "elf";
}


/* arch_get_instruction */
ArchInstruction * arch_get_instruction(Arch * arch, size_t index)
{
	if(index >= arch->instructions_cnt)
		return NULL;
	return &arch->plugin->instructions[index];
}


/* arch_get_instruction_by_name */
ArchInstruction * arch_get_instruction_by_name(Arch * arch, char const * name)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, \"%s\")\n", __func__, name);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
		if(strcmp(arch->plugin->instructions[i].name, name) == 0)
			return &arch->plugin->instructions[i];
	return NULL;
}


/* arch_get_instruction_by_opcode */
ArchInstruction * arch_get_instruction_by_opcode(Arch * arch, uint8_t size,
		uint32_t opcode)
{
	size_t i;
	ArchInstruction * ai;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, %u, 0x%x)\n", __func__, size, opcode);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
	{
		ai = &arch->plugin->instructions[i];
		if(AO_GET_SIZE(ai->flags) != size)
			continue;
		if(ai->opcode == opcode)
			return ai;
	}
	return NULL;
}


/* arch_get_instruction_by_call */
static int _call_operands(Arch * arch, ArchInstruction * instruction,
		ArchInstructionCall * call);
static int _call_operands_constant(ArchOperandDefinition definition,
		ArchOperand * operand);
static int _call_operands_dregister(Arch * arch,
		ArchOperandDefinition definition, ArchOperand * operand);
static int _call_operands_immediate(ArchOperandDefinition definition,
		ArchOperand * operand);
static int _call_operands_register(Arch * arch,
		ArchOperandDefinition definition, ArchOperand * operand);

ArchInstruction * arch_get_instruction_by_call(Arch * arch,
		ArchInstructionCall * call)
{
	size_t i;
	ArchInstruction * ai;
	int found = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, call->name);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
	{
		ai = &arch->plugin->instructions[i];
		/* FIXME use a (sorted) hash table */
		if(strcmp(ai->name, call->name) != 0)
			continue;
		found = 1;
		if(_call_operands(arch, ai, call) == 0)
			return ai;
	}
	error_set_code(1, "%s \"%s\"", found ? "Invalid arguments to"
			: "Unknown instruction", call->name);
	return NULL;
}

static int _call_operands(Arch * arch, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	size_t i;
	ArchOperandDefinition definition;
	ArchOperand * operand;

	if(call->operands_cnt == 0 && AO_GET_TYPE(instruction->op1) != AOT_NONE)
		return -1;
	for(i = 0; i < call->operands_cnt; i++)
	{
		definition = (i == 0) ? instruction->op1 : ((i == 1)
				? instruction->op2 : instruction->op3);
		operand = &call->operands[i];
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() operand %lu, type %u, type %u\n",
				__func__, i, AO_GET_TYPE(definition),
				AO_GET_TYPE(operand->type));
#endif
		if(AO_GET_TYPE(definition) == AOT_CONSTANT)
		{
			if(operand->type != AOT_IMMEDIATE)
				return -1;
		}
		else if(AO_GET_TYPE(definition) != operand->type)
			return -1;
		switch(AO_GET_TYPE(definition))
		{
			case AOT_CONSTANT:
				if(_call_operands_constant(definition, operand)
						!= 0)
					return -1;
				break;
			case AOT_IMMEDIATE:
				if(_call_operands_immediate(definition, operand)
						!= 0)
					return -1;
				break;
			case AOT_DREGISTER:
				if(_call_operands_dregister(arch, definition,
							operand) != 0)
					return -1;
				break;
			case AOT_REGISTER:
				if(_call_operands_register(arch, definition,
							operand) != 0)
					return -1;
				break;
		}
	}
	return 0;
}

static int _call_operands_constant(ArchOperandDefinition definition,
		ArchOperand * operand)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %u %lu\n", __func__,
			AO_GET_VALUE(definition),
			operand->value.immediate.value);
#endif
	if(AO_GET_VALUE(definition) != operand->value.immediate.value)
		return -1;
	/* set this operand as a constant */
	operand->type &= AOM_TYPE;
	operand->type |= AOT_CONSTANT;
	return 0;
}

static int _call_operands_dregister(Arch * arch,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	uint64_t offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %ld\n", __func__,
			operand->value.dregister.offset);
#endif
	if(_call_operands_register(arch, definition, operand) != 0)
		return -1;
	/* check if there is an offset applied */
	if(operand->value.dregister.offset == 0)
		return 0;
	/* check if the offset fits */
	offset = abs(operand->value.dregister.offset);
	offset >>= AO_GET_DSIZE(definition);
	if(offset > 0)
		return -1;
	return 0;
}

static int _call_operands_immediate(ArchOperandDefinition definition,
		ArchOperand * operand)
{
	uint64_t value;
	uint32_t size;

	/* check if the size fits */
	value = operand->value.immediate.value;
#if 0 /* XXX ignore for now */
	if((size = AO_GET_SIZE(definition)) > 0
			&& AO_GET_FLAGS(definition) & AOF_SIGNED)
		size--;
#else
	size = AO_GET_SIZE(definition);
#endif
	value >>= size;
	if(value > 0)
		return -1;
	/* check if it is signed */
	if(operand->value.immediate.negative
			&& !(AO_GET_FLAGS(definition) & AOF_SIGNED))
		return -1;
	return 0;
}

static int _call_operands_register(Arch * arch,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	char const * name = operand->value._register.name;
	ArchDescription * desc;
	uint32_t size;
	ArchRegister * ar;

	/* obtain the size */
	if((desc = arch->plugin->description) != NULL
			&& desc->instruction_size != 0)
		size = desc->instruction_size;
	else
		size = AO_GET_SIZE(definition);
	/* check if it exists */
	if((ar = arch_get_register_by_name_size(arch, name, size)) == NULL)
		return -1;
	/* for implicit instructions it must match */
	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT
			&& AO_GET_VALUE(definition) != ar->id)
		return -1;
	return 0;
}


/* arch_get_name */
char const * arch_get_name(Arch * arch)
{
	return arch->plugin->name;
}


/* arch_get_register */
ArchRegister * arch_get_register(Arch * arch, size_t index)
{
	if(index >= arch->registers_cnt)
		return NULL;
	return &arch->plugin->registers[index];
}


/* arch_get_register_by_id_size */
ArchRegister * arch_get_register_by_id_size(Arch * arch, uint32_t id,
		uint32_t size)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, %u)\n", __func__, id, size);
#endif
	for(i = 0; i < arch->registers_cnt; i++)
		if(arch->plugin->registers[i].id == id
				&& arch->plugin->registers[i].size == size)
			return &arch->plugin->registers[i];
	return NULL;
}


/* arch_get_register_by_name */
ArchRegister * arch_get_register_by_name(Arch * arch, char const * name)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	for(i = 0; i < arch->registers_cnt; i++)
		if(strcmp(arch->plugin->registers[i].name, name) == 0)
			return &arch->plugin->registers[i];
	return NULL;
}


/* arch_get_register_by_name_size */
ArchRegister * arch_get_register_by_name_size(Arch * arch, char const * name,
		uint32_t size)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %u)\n", __func__, name, size);
#endif
	for(i = 0; i < arch->registers_cnt; i++)
		if(arch->plugin->registers[i].size != size)
			continue;
		else if(strcmp(arch->plugin->registers[i].name, name) == 0)
			return &arch->plugin->registers[i];
	return NULL;
}


/* useful */
/* arch_decode */
static void _decode_print(off_t offset, ArchInstructionCall * call);

int arch_decode(Arch * arch)
{
	ArchInstructionCall call;
	off_t offset = arch->buffer_pos;

	if(arch->plugin->decode == NULL)
		return -error_set_code(1, "%s: %s", arch->plugin->name,
				"Disassembly not supported");
	printf("\n%08lx:\n", offset);
	for(; arch->plugin->decode(arch->plugin, &call) == 0;
			offset = arch->buffer_pos)
		_decode_print(offset, &call);
	return 0;
}

static void _decode_print(off_t offset, ArchInstructionCall * call)
{
	char const * sep = " ";
	size_t i;
	ArchOperand * ao;
	char const * name;

	printf("%8lx: %-12s", offset, call->name);
	for(i = 0; i < call->operands_cnt; i++)
	{
		ao = &call->operands[i];
		switch(AO_GET_TYPE(call->operands[i].type))
		{
			case AOT_DREGISTER:
				name = ao->value.dregister.name;
				printf("%s[%%%s + $0x%lx]", sep, name,
						ao->value.dregister.offset);
				break;
			case AOT_DREGISTER2:
				name = ao->value.dregister2.name;
				printf("%s[%%%s + %%%s]", sep, name,
						ao->value.dregister2.name2);
				break;
			case AOT_IMMEDIATE:
				printf("%s%s$0x%lx", sep,
						ao->value.immediate.negative
						? "-" : "",
						ao->value.immediate.value);
				break;
			case AOT_REGISTER:
				name = call->operands[i].value._register.name;
				printf("%s%%%s", sep, name);
				break;
		}
		sep = ", ";
	}
	putchar('\n');
}


/* arch_decode_at */
int arch_decode_at(Arch * arch, off_t offset, size_t size, off_t base)
{
	int ret;

	/* FIXME this only works for files */
	if(arch->fp == NULL)
		return -error_set_code(1, "%s", strerror(ENOSYS));
	if(fseek(arch->fp, offset, SEEK_SET) != 0)
		return -error_set_code(1, "%s", strerror(errno));
	if(size == 0)
		return 0;
	arch->buffer_pos = offset + base;
	arch->buffer_cnt = offset + base + size;
	if((ret = arch_decode(arch)) == 0
			&& fseek(arch->fp, offset + size, SEEK_SET) != 0)
		ret = -error_set_code(1, "%s", strerror(errno));
	return ret;
}


/* arch_exit */
int arch_exit(Arch * arch)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	arch->filename = NULL;
	arch->fp = NULL;
	arch->buffer = NULL;
	arch->buffer_cnt = 0;
	arch->buffer_pos = 0;
	memset(&arch->helper, 0, sizeof(arch->helper));
	return 0;
}


/* arch_init */
int arch_init(Arch * arch, char const * filename, FILE * fp)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename,
			(void *)fp);
#endif
	arch->filename = filename;
	arch->fp = fp;
	arch->buffer_pos = 0; /* XXX used as offset */
	arch->helper.arch = arch;
	arch->helper.get_filename = _arch_get_filename;
	arch->helper.get_instruction_by_opcode = arch_get_instruction_by_opcode;
	arch->helper.get_register_by_id_size = arch_get_register_by_id_size;
	arch->helper.get_register_by_name_size = arch_get_register_by_name_size;
	arch->helper.read = _arch_read;
	arch->helper.write = _arch_write;
	arch->plugin->helper = &arch->helper;
	return 0;
}


/* arch_init */
int arch_init_buffer(Arch * arch, char const * buffer, size_t size)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	arch->filename = "buffer";
	arch->buffer = buffer;
	arch->buffer_cnt = size;
	arch->buffer_pos = 0;
	arch->helper.arch = arch;
	arch->helper.get_filename = _arch_get_filename;
	arch->helper.get_instruction_by_opcode = arch_get_instruction_by_opcode;
	arch->helper.get_register_by_id_size = arch_get_register_by_id_size;
	arch->helper.get_register_by_name_size = arch_get_register_by_name_size;
	arch->helper.write = NULL;
	arch->helper.read = _arch_read_buffer;
	arch->plugin->helper = &arch->helper;
	return 0;
}


/* arch_write */
int arch_write(Arch * arch, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	return arch->plugin->write(arch->plugin, instruction, call);
}


/* private */
/* callbacks */
/* arch_get_filename */
static char const * _arch_get_filename(Arch * arch)
{
	return arch->filename;
}


/* arch_read */
static ssize_t _arch_read(Arch * arch, void * buf, size_t size)
{
	size_t s = min(arch->buffer_cnt - arch->buffer_pos, size);

	if(fread(buf, s, 1, arch->fp) == 1)
	{
		arch->buffer_pos += s;
		return s;
	}
	if(ferror(arch->fp))
		return -error_set_code(1, "%s: %s", arch->filename,
				strerror(errno));
	if(feof(arch->fp))
		return -error_set_code(1, "%s: %s", arch->filename,
				"End of file reached");
	return -error_set_code(1, "%s: %s", arch->filename, "Read error");
}


/* arch_read_buffer */
static ssize_t _arch_read_buffer(Arch * arch, void * buf, size_t size)
{
	ssize_t s = min(arch->buffer_cnt - arch->buffer_pos, size);

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(s == 0)
		return -error_set_code(1, "%s", "End of buffer reached");
	memcpy(buf, &arch->buffer[arch->buffer_pos], s);
	arch->buffer_pos += s;
	return s;
}


/* arch_write */
static ssize_t _arch_write(Arch * arch, void const * buf, size_t size)
{
	if(fwrite(buf, size, 1, arch->fp) == 1)
		return size;
	if(ferror(arch->fp))
		return -error_set_code(1, "%s: %s", arch->filename,
				strerror(errno));
	if(feof(arch->fp))
		return -error_set_code(1, "%s: %s", arch->filename,
				"End of file reached");
	return -error_set_code(1, "%s: %s", arch->filename, "Write error");
}
