/* $Id$ */
/* Copyright (c) 2011-2017 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel Asm */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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


/* AsmArch */
/* private */
/* types */
struct _AsmArch
{
	AsmArchPluginHelper helper;
	Plugin * handle;
	AsmArchPluginDefinition * definition;
	AsmArchPlugin * plugin;
	size_t instructions_cnt;
	size_t registers_cnt;

	/* internal */
	AsmCode * code;
	off_t base;
	char const * filename;
	FILE * fp;
	char const * buffer;
	size_t buffer_cnt;
	size_t buffer_pos;
};


/* prototypes */
/* callbacks */
static char const * _arch_get_filename(AsmArch * arch);
static AsmFunction * _arch_get_function_by_id(AsmArch * arch, AsmFunctionId id);
static AsmString * _arch_get_string_by_id(AsmArch * arch, AsmStringId id);
static ssize_t _arch_peek(AsmArch * arch, void * buf, size_t size);
static ssize_t _arch_read(AsmArch * arch, void * buf, size_t size);
static ssize_t _arch_peek_buffer(AsmArch * arch, void * buf, size_t size);
static ssize_t _arch_read_buffer(AsmArch * arch, void * buf, size_t size);
static off_t _arch_seek(AsmArch * arch, off_t offset, int whence);
static off_t _arch_seek_buffer(AsmArch * arch, off_t offset, int whence);
static ssize_t _arch_write(AsmArch * arch, void const * buf, size_t size);


/* public */
/* functions */
#ifndef STANDALONE
/* arch_new */
AsmArch * arch_new(char const * name)
{
	AsmArch * a;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	if((a = object_new(sizeof(*a))) == NULL)
		return NULL;
	memset(&a->helper, 0, sizeof(a->helper));
	if((a->handle = plugin_new(LIBDIR, PACKAGE, "arch", name)) == NULL
			|| (a->definition = plugin_lookup(a->handle,
					"arch_plugin")) == NULL)
	{
		if(a->handle != NULL)
			plugin_delete(a->handle);
		object_delete(a);
		return NULL;
	}
	a->plugin = NULL;
#if 1 /* XXX should be done when initializing */
	a->instructions_cnt = 0;
	if(a->definition->instructions != NULL)
		for(; a->definition->instructions[a->instructions_cnt].name
				!= NULL; a->instructions_cnt++);
	a->registers_cnt = 0;
	if(a->definition->registers != NULL)
		for(; a->definition->registers[a->registers_cnt].name != NULL;
				a->registers_cnt++);
#endif
	a->filename = NULL;
	a->fp = NULL;
	a->buffer = NULL;
	a->buffer_cnt = 0;
	a->buffer_pos = 0;
	return a;
}
#endif


/* arch_delete */
void arch_delete(AsmArch * arch)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	plugin_delete(arch->handle);
	object_delete(arch);
}


/* accessors */
/* arch_can_decode */
int arch_can_decode(AsmArch * arch)
{
	return (arch->definition->decode != NULL) ? 1 : 0;
}


/* arch_get_definition */
AsmArchDefinition const * arch_get_definition(AsmArch * arch)
{
	return arch->definition->definition;
}


/* arch_get_description */
char const * arch_get_description(AsmArch * arch)
{
	return arch->definition->description;
}


/* arch_get_format */
char const * arch_get_format(AsmArch * arch)
{
	if(arch->definition->definition != NULL
			&& arch->definition->definition->format != NULL)
		return arch->definition->definition->format;
	return NULL;
}


/* arch_get_instruction */
AsmArchInstruction const * arch_get_instruction(AsmArch * arch, size_t index)
{
	if(index >= arch->instructions_cnt)
		return NULL;
	return &arch->definition->instructions[index];
}


/* arch_get_instruction_by_name */
AsmArchInstruction const * arch_get_instruction_by_name(AsmArch * arch,
		char const * name)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, \"%s\")\n", __func__, name);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
		if(strcmp(arch->definition->instructions[i].name, name) == 0)
			return &arch->definition->instructions[i];
	return NULL;
}


/* arch_get_instruction_by_opcode */
AsmArchInstruction const * arch_get_instruction_by_opcode(AsmArch * arch,
		uint8_t size, uint32_t opcode)
{
	size_t i;
	AsmArchInstruction const * ai;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, %u, 0x%x)\n", __func__, size, opcode);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
	{
		ai = &arch->definition->instructions[i];
		if(AO_GET_SIZE(ai->flags) != size)
			continue;
		if(ai->opcode == opcode)
			return ai;
	}
	return NULL;
}


/* arch_get_instruction_by_call */
static int _call_operands(AsmArch * arch,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall * call);
static int _call_operands_constant(AsmArchOperandDefinition definition,
		AsmArchOperand * operand);
static int _call_operands_dregister(AsmArch * arch,
		AsmArchOperandDefinition definition, AsmArchOperand * operand);
static int _call_operands_immediate(AsmArchOperandDefinition definition,
		AsmArchOperand * operand);
static int _call_operands_register(AsmArch * arch,
		AsmArchOperandDefinition definition, AsmArchOperand * operand);

AsmArchInstruction const * arch_get_instruction_by_call(AsmArch * arch,
		AsmArchInstructionCall * call)
{
	size_t i;
	AsmArchInstruction const * ai;
	int found = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, call->name);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
	{
		ai = &arch->definition->instructions[i];
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

static int _call_operands(AsmArch * arch,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall * call)
{
	size_t i;
	AsmArchOperandDefinition definition;
	AsmArchOperand * operand;

	if(call->operands_cnt == 0 && AO_GET_TYPE(instruction->op1) != AOT_NONE)
		return -1;
	for(i = 0; i < call->operands_cnt; i++)
	{
		switch(i)
		{
			case 0:	definition = instruction->op1; break;
			case 1:	definition = instruction->op2; break;
			case 2:	definition = instruction->op3; break;
			case 3:	definition = instruction->op4; break;
			case 4:	definition = instruction->op5; break;
			default:
				return -1;
		}
		operand = &call->operands[i];
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() operand %lu, type %u, type %u\n",
				__func__, i, AO_GET_TYPE(definition),
				AO_GET_TYPE(operand->definition));
#endif
		if(AO_GET_TYPE(definition) == AOT_CONSTANT)
		{
			if(AO_GET_TYPE(operand->definition) != AOT_IMMEDIATE)
				return -1;
		}
		else if(AO_GET_TYPE(definition)
				!= AO_GET_TYPE(operand->definition))
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

static int _call_operands_constant(AsmArchOperandDefinition definition,
		AsmArchOperand * operand)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %u %lu\n", __func__,
			AO_GET_VALUE(definition),
			operand->value.immediate.value);
#endif
	if(AO_GET_VALUE(definition) != operand->value.immediate.value)
		return -1;
	/* set this operand as a constant */
	operand->definition &= AOM_TYPE;
	operand->definition |= (AOT_CONSTANT << AOD_TYPE);
	return 0;
}

static int _call_operands_dregister(AsmArch * arch,
		AsmArchOperandDefinition definition, AsmArchOperand * operand)
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

static int _call_operands_immediate(AsmArchOperandDefinition opdefinition,
		AsmArchOperand * operand)
{
	uint64_t value;
	uint32_t size;

	/* check if the size fits */
	value = operand->value.immediate.value;
#if 0 /* XXX ignore for now */
	if((size = AO_GET_SIZE(opdefinition)) > 0
			&& AO_GET_FLAGS(opdefinition) & AOF_SIGNED)
		size--;
#else
	size = AO_GET_SIZE(opdefinition);
#endif
	value >>= size;
	if(value > 0)
		return -1;
	/* check if it is signed */
	if(operand->value.immediate.negative
			&& !(AO_GET_FLAGS(opdefinition) & AOF_SIGNED))
		return -1;
	return 0;
}

static int _call_operands_register(AsmArch * arch,
		AsmArchOperandDefinition opdefinition, AsmArchOperand * operand)
{
	char const * name = operand->value._register.name;
	AsmArchDefinition const * definition;
	uint32_t size;
	AsmArchRegister const * ar;

	/* obtain the size */
	if((definition = arch->definition->definition) != NULL
			&& definition->instruction_size != 0)
		size = definition->instruction_size;
	else
		size = AO_GET_SIZE(opdefinition);
	/* check if it exists */
	if((ar = arch_get_register_by_name_size(arch, name, size)) == NULL)
		return -1;
	/* for implicit instructions it must match */
	if(AO_GET_FLAGS(opdefinition) & AOF_IMPLICIT
			&& AO_GET_VALUE(opdefinition) != ar->id)
		return -1;
	return 0;
}


/* arch_get_instructions */
AsmArchInstruction const * arch_get_instructions(AsmArch * arch)
{
	return arch->definition->instructions;
}


/* arch_get_name */
char const * arch_get_name(AsmArch * arch)
{
	return arch->definition->name;
}


/* arch_get_register */
AsmArchRegister const * arch_get_register(AsmArch * arch, size_t index)
{
	if(index >= arch->registers_cnt)
		return NULL;
	return &arch->definition->registers[index];
}


/* arch_get_register_by_id_size */
AsmArchRegister const * arch_get_register_by_id_size(AsmArch * arch,
		uint32_t id, uint32_t size)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, %u)\n", __func__, id, size);
#endif
	for(i = 0; i < arch->registers_cnt; i++)
		if(arch->definition->registers[i].id == id
				&& arch->definition->registers[i].size == size)
			return &arch->definition->registers[i];
	return NULL;
}


/* arch_get_register_by_name */
AsmArchRegister const * arch_get_register_by_name(AsmArch * arch,
		char const * name)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	for(i = 0; i < arch->registers_cnt; i++)
		if(strcmp(arch->definition->registers[i].name, name) == 0)
			return &arch->definition->registers[i];
	return NULL;
}


/* arch_get_register_by_name_size */
AsmArchRegister const * arch_get_register_by_name_size(AsmArch * arch,
		char const * name, uint32_t size)
{
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %u)\n", __func__, name, size);
#endif
	for(i = 0; i < arch->registers_cnt; i++)
		if(arch->definition->registers[i].size != size)
			continue;
		else if(strcmp(arch->definition->registers[i].name, name) == 0)
			return &arch->definition->registers[i];
	return NULL;
}


/* arch_get_registers */
AsmArchRegister const * arch_get_registers(AsmArch * arch)
{
	return arch->definition->registers;
}


/* useful */
/* arch_decode */
int arch_decode(AsmArch * arch, AsmCode * code, off_t base,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	int ret = 0;
	AsmArchInstructionCall * c;
	size_t c_cnt;
	AsmArchInstructionCall * p;
	size_t offset = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%ld)\n", __func__, base);
#endif
	if(arch->definition->decode == NULL)
		return -error_set_code(1, "%s: %s", arch->definition->name,
				"Disassembly not supported");
	/* check the arguments */
	if(calls == NULL || calls_cnt == NULL)
		return -error_set_code(1, "%s: %s", arch->definition->name,
				strerror(EINVAL));
	c = *calls;
	c_cnt = *calls_cnt;
	arch->code = code;
	for(;;)
	{
		if((p = realloc(c, sizeof(*c) * (c_cnt + 1))) == NULL)
		{
			free(c);
			ret = -error_set_code(1, "%s", strerror(errno));
			break;
		}
		c = p;
		p = &c[c_cnt];
		memset(p, 0, sizeof(*p));
		p->base = base + offset;
		p->offset = arch->buffer_pos;
		if(arch->definition->decode(arch->plugin, p) != 0)
			break;
		p->size = arch->buffer_pos - p->offset;
		offset += p->size;
		c_cnt++;
	}
	*calls = c;
	*calls_cnt = c_cnt;
	arch->code = NULL;
	return ret;
}


/* arch_decode_at */
int arch_decode_at(AsmArch * arch, AsmCode * code, off_t offset, size_t size,
		off_t base, AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%ld, %lu, %ld)\n", __func__, offset, size,
			base);
#endif
	/* FIXME this only works for files */
	if(arch->fp == NULL)
		return -error_set_code(1, "%s", strerror(ENOSYS));
	if(fseek(arch->fp, offset, SEEK_SET) != 0)
		return -error_set_code(1, "%s", strerror(errno));
	if(size == 0)
		return 0;
	arch->code = code;
	arch->buffer_pos = offset;
	arch->buffer_cnt = offset + size;
	if((ret = arch_decode(arch, code, base, calls, calls_cnt)) == 0
			&& fseek(arch->fp, offset + size, SEEK_SET) != 0)
	{
		free(*calls); /* XXX the pointer was updated anyway... */
		ret = -error_set_code(1, "%s", strerror(errno));
	}
	return ret;
}


/* arch_encode */
int arch_encode(AsmArch * arch, AsmArchInstruction const * instruction,
		AsmArchInstructionCall * call)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	return arch->definition->encode(arch->plugin, instruction, call);
}


/* arch_exit */
int arch_exit(AsmArch * arch)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(arch->plugin != NULL)
		arch->definition->destroy(arch->plugin);
	arch->plugin = NULL;
	arch->filename = NULL;
	arch->fp = NULL;
	arch->buffer = NULL;
	arch->buffer_cnt = 0;
	arch->buffer_pos = 0;
	memset(&arch->helper, 0, sizeof(arch->helper));
	return 0;
}


/* arch_init */
int arch_init(AsmArch * arch, char const * filename, FILE * fp)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %p)\n", __func__, filename,
			(void *)fp);
#endif
	if(arch->plugin != NULL)
		arch_exit(arch);
	arch->base = 0;
	arch->filename = filename;
	arch->fp = fp;
	arch->buffer = NULL;
	arch->buffer_cnt = 0;
	arch->buffer_pos = 0; /* XXX used as offset */
	arch->helper.arch = arch;
	arch->helper.get_filename = _arch_get_filename;
	arch->helper.get_function_by_id = _arch_get_function_by_id;
	arch->helper.get_instruction_by_opcode = arch_get_instruction_by_opcode;
	arch->helper.get_register_by_id_size = arch_get_register_by_id_size;
	arch->helper.get_register_by_name_size = arch_get_register_by_name_size;
	arch->helper.get_string_by_id = _arch_get_string_by_id;
	arch->helper.peek = _arch_peek;
	arch->helper.read = _arch_read;
	arch->helper.seek = _arch_seek;
	arch->helper.write = _arch_write;
	if(arch->definition->init != NULL
			&& (arch->plugin = arch->definition->init(
					&arch->helper)) == NULL)
		return -1;
	return 0;
}


/* arch_init */
int arch_init_buffer(AsmArch * arch, char const * buffer, size_t size)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(arch->plugin != NULL)
		arch_exit(arch);
	arch->base = 0;
	arch->filename = "buffer";
	arch->fp = NULL;
	arch->buffer = buffer;
	arch->buffer_cnt = size;
	arch->buffer_pos = 0;
	arch->helper.arch = arch;
	arch->helper.get_filename = _arch_get_filename;
	arch->helper.get_function_by_id = _arch_get_function_by_id;
	arch->helper.get_instruction_by_opcode = arch_get_instruction_by_opcode;
	arch->helper.get_register_by_id_size = arch_get_register_by_id_size;
	arch->helper.get_register_by_name_size = arch_get_register_by_name_size;
	arch->helper.get_string_by_id = _arch_get_string_by_id;
	arch->helper.write = NULL;
	arch->helper.peek = _arch_peek_buffer;
	arch->helper.read = _arch_read_buffer;
	arch->helper.seek = _arch_seek_buffer;
	if(arch->definition->init != NULL
			&& (arch->plugin = arch->definition->init(
					&arch->helper)) == NULL)
		return -1;
	return 0;
}


/* arch_read */
ssize_t arch_read(AsmArch * arch, void * buf, size_t size)
{
	if(arch->helper.read == NULL)
		return -error_set_code(1, "%s", "read: No helper defined");
	return arch->helper.read(arch, buf, size);
}


/* arch_seek */
off_t arch_seek(AsmArch * arch, off_t offset, int whence)
{
	if(arch->helper.seek == NULL)
		return -error_set_code(1, "%s", "seek: No helper defined");
	return arch->helper.seek(arch, offset, whence);
}


/* private */
/* callbacks */
/* arch_get_filename */
static char const * _arch_get_filename(AsmArch * arch)
{
	return arch->filename;
}


/* arch_get_function_by_id */
static AsmFunction * _arch_get_function_by_id(AsmArch * arch, AsmFunctionId id)
{
	return asmcode_get_function_by_id(arch->code, id);
}


/* arch_get_string_by_id */
static AsmString * _arch_get_string_by_id(AsmArch * arch, AsmStringId id)
{
	return asmcode_get_string_by_id(arch->code, id);
}


/* arch_peek */
static ssize_t _arch_peek(AsmArch * arch, void * buf, size_t size)
{
	ssize_t s;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, %p, %lu)\n", __func__, buf, size);
#endif
	if((s = _arch_read(arch, buf, size)) == -1)
		return -1;
	if(_arch_seek(arch, -s, SEEK_CUR) == -1)
		return -1;
	return s;
}


/* arch_peek_buffer */
static ssize_t _arch_peek_buffer(AsmArch * arch, void * buf, size_t size)
{
	ssize_t s;

	if((s = _arch_read_buffer(arch, buf, size)) == -1)
		return -1;
	if(_arch_seek_buffer(arch, -s, SEEK_CUR) == -1)
		return -1;
	return s;
}


/* arch_read */
static ssize_t _arch_read(AsmArch * arch, void * buf, size_t size)
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
static ssize_t _arch_read_buffer(AsmArch * arch, void * buf, size_t size)
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


/* arch_seek */
static off_t _arch_seek(AsmArch * arch, off_t offset, int whence)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, %ld, %d)\n", __func__, offset, whence);
#endif
	if(fseek(arch->fp, offset, whence) != 0)
		return -error_set_code(1, "%s: %s", arch->filename, strerror(
					errno));
	arch->buffer_pos = ftello(arch->fp);
	return arch->buffer_pos;
}


/* arch_seek_buffer */
static off_t _arch_seek_buffer(AsmArch * arch, off_t offset, int whence)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(arch, %ld, %d)\n", __func__, offset, whence);
#endif
	if(whence == SEEK_SET)
	{
		if(offset < 0 || (size_t)offset >= arch->buffer_cnt)
			return -error_set_code(1, "%s", "Invalid seek");
		arch->buffer_pos = offset;
	}
	else if(whence == SEEK_CUR)
	{
		if(offset < 0)
		{
			if((size_t)(-offset) > arch->buffer_pos)
				return -error_set_code(1, "%s", "Invalid seek");
		}
		else if(offset > 0)
		{
			if((size_t)offset + arch->buffer_pos
					>= arch->buffer_cnt)
				return -error_set_code(1, "%s", "Invalid seek");
		}
		arch->buffer_pos += offset;
	}
	else
		/* FIXME implement */
		return -error_set_code(1, "%s", "Not implemented");
	return arch->buffer_pos;
}


/* arch_write */
static ssize_t _arch_write(AsmArch * arch, void const * buf, size_t size)
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
