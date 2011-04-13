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
#include <assert.h>
#include "Asm/arch.h"
#include "Asm/asm.h"
#include "arch.h"
#include "../config.h"


/* Arch */
/* private */
/* types */
struct _Arch
{
	Plugin * handle;
	ArchPlugin * plugin;
	size_t instructions_cnt;
	size_t registers_cnt;
};


/* macros */
#define AI_GET_OPERAND_COUNT(ai) (((ai->op1 & AOM_TYPE) ? 1 : 0) + \
		((ai->op2 & AOM_TYPE) ? 1 : 0) + \
		((ai->op3 & AOM_TYPE) ? 1 : 0))


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
	if(arch->plugin->format != NULL)
		return arch->plugin->format;
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
		if(AO_GET_SIZE(ai->opcode) != size)
			continue;
		if(ai->value == opcode)
			return ai;
	}
#if 0
		if(arch->instructions[i].size == size
				&& arch->instructions[i].opcode == opcode)
			return &arch->instructions[i];
# if 0 /* XXX this is experimental and may not be adequate */
	for(i = 0; i < arch->instructions_cnt; i++)
		if(arch->instructions[i].size == 0
				&& arch->instructions[i].op1size == size)
			return &arch->instructions[i];
# endif
#endif
	return NULL;
}


/* arch_get_instruction_by_operands */
static int _operands_operands(ArchInstruction * ai, AsOperand ** operands,
		size_t operands_cnt);

ArchInstruction * arch_get_instruction_by_operands(Arch * arch,
		char const * name, AsOperand ** operands, size_t operands_cnt)
{
	size_t i;
	ArchInstruction * ai;
	int found = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	for(i = 0; i < arch->instructions_cnt; i++)
	{
		ai = &arch->plugin->instructions[i];
		/* FIXME use a (sorted) hash table */
		if(strcmp(ai->name, name) != 0)
			continue;
		found = 1;
		if(_operands_operands(ai, operands, operands_cnt) == 0)
			return ai;
	}
	error_set_code(1, "%s \"%s\"", found ? "Invalid arguments to"
			: "Unknown instruction", name);
	return NULL;
}

static int _operands_operands(ArchInstruction * ai, AsOperand ** operands,
		size_t operands_cnt)
{
	size_t i;
	uint32_t operand;

	for(i = 0; i < operands_cnt; i++)
	{
		if(i >= 3)
			return -1;
		operand = (i == 0) ? ai->op1 : ((i == 1) ? ai->op2 : ai->op3);
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() %lu %u, %u\n", __func__, i,
				AO_GET_TYPE(operand),
				AO_GET_TYPE(operands[i]->operand));
#endif
		if(AO_GET_TYPE(operand) != AO_GET_TYPE(operands[i]->operand))
			return -1;
		/* FIXME check AOF_SIGNED */
	}
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


/* arch_get_register_by_id */
ArchRegister * arch_get_register_by_id(Arch * arch, unsigned int id)
{
	size_t i;

	for(i = 0; i < arch->registers_cnt; i++)
		if(arch->plugin->registers[i].id == id)
			return &arch->plugin->registers[i];
	return NULL;
}


/* arch_get_register_by_name */
ArchRegister * arch_get_register_by_name(Arch * arch, char const * name)
{
	size_t i;

	for(i = 0; i < arch->registers_cnt; i++)
		if(strcmp(arch->plugin->registers[i].name, name) == 0)
			return &arch->plugin->registers[i];
	return NULL;
}
