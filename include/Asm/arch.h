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



#ifndef DEVEL_ASM_ARCH_H
# define DEVEL_ASM_ARCH_H

# include <sys/types.h>
# include <stdint.h>
# include "asm.h"


/* AsmArch */
/* types */
typedef struct _Arch Arch;

typedef enum _ArchEndian
{
	ARCH_ENDIAN_BIG		= 0x1,
	ARCH_ENDIAN_LITTLE	= 0x2,
	ARCH_ENDIAN_BOTH	= 0x3
} ArchEndian;

typedef struct _ArchDescription
{
	char const * format;		/* default format plug-in */
	ArchEndian endian;
	uint32_t address_size;
	uint32_t alignment;
	uint32_t instruction_size;	/* 0 if not constant */
} ArchDescription;

/* operands */
typedef enum _ArchOperandType
{
	AOT_NONE	= 0x0,
	AOT_CONSTANT	= 0x1,		/* flags |      0 |   size |  value */
	AOT_IMMEDIATE	= 0x2,		/* flags |      0 |   size | refers */
	AOT_REGISTER	= 0x3,		/* flags |      0 |   size |     id */
	AOT_DREGISTER	= 0x4,		/* flags |  dsize |  rsize |     id */
	AOT_DREGISTER2	= 0x5		/* flags |    did |  rsize |     id */
} ArchOperandType;

/* displacement */
# define AOD_FLAGS	24
# define AOD_OFFSET	16
# define AOD_SIZE	8
# define AOD_TYPE	28
# define AOD_VALUE	0

/* masks */
# define AOM_TYPE	0xf0000000
# define AOM_FLAGS	0x0f000000
# define AOM_OFFSET	0x00ff0000
# define AOM_SIZE	0x0000ff00
# define AOM_VALUE	0x000000ff

/* flags */
/* constants */
# define AOF_IMPLICIT	0x1
/* for immediate */
# define AOF_SIGNED	0x1
/* for registers */
# define AOF_IMPLICIT	0x1

/* immediate refers */
# define AOI_REFERS_STRING	0x1
# define AOI_REFERS_FUNCTION	0x2

/* macros */
# define AO_0()		AOT_NONE, AOT_NONE, AOT_NONE
# define AO_1(op1)	op1, AOT_NONE, AOT_NONE
# define AO_2(op1, op2)	op1, op2, AOT_NONE
# define AO_3(op1, op2, op3) \
			op1, op2, op3
# define AO_GET_FLAGS(operand)	((operand & AOM_FLAGS) >> AOD_FLAGS)
# define AO_GET_OFFSET(operand)	((operand & AOM_OFFSET) >> AOD_OFFSET)
# define AO_GET_DSIZE(operand)	((operand & AOM_OFFSET) >> AOD_OFFSET)
# define AO_GET_RSIZE(operand)	((operand & AOM_SIZE) >> AOD_SIZE)
# define AO_GET_SIZE(operand)	((operand & AOM_SIZE) >> AOD_SIZE)
# define AO_GET_TYPE(operand)	((operand & AOM_TYPE) >> AOD_TYPE)
# define AO_GET_VALUE(operand)	((operand & AOM_VALUE) >> AOD_VALUE)

# define AO_CONSTANT(flags, size, value) \
		((AOT_CONSTANT << AOD_TYPE) \
		 | ((flags) << AOD_FLAGS) \
		 | ((size) << AOD_SIZE) \
		 | ((value) << AOD_VALUE))
# define AO_IMMEDIATE(flags, size, type) \
		((AOT_IMMEDIATE << AOD_TYPE) \
		 | ((flags) << AOD_FLAGS) \
		 | ((size) << AOD_SIZE) \
		 | ((type) << AOD_VALUE))
# define AO_REGISTER(flags, size, id) \
		((AOT_REGISTER << AOD_TYPE) \
		 | ((flags) << AOD_FLAGS) \
		 | ((size) << AOD_SIZE) \
		 | ((id) << AOD_VALUE))
# define AO_DREGISTER(flags, dsize, rsize, id) \
		((AOT_DREGISTER << AOD_TYPE) \
		 | ((flags) << AOD_FLAGS) \
		 | ((dsize) << AOD_OFFSET) \
		 | ((rsize) << AOD_SIZE) \
		 | ((id) << AOD_VALUE))
# define AO_DREGISTER2(flags, did, dsize, id) \
		((AOT_DREGISTER2 << AOD_TYPE) \
		 | ((flags) << AOD_FLAGS) \
		 | ((did) << AOD_OFFSET) \
		 | ((dsize) << AOD_SIZE) \
		 | ((id) << AOD_VALUE))

typedef uint32_t ArchOperandDefinition;

typedef struct _ArchOperand
{
	ArchOperandDefinition definition;
	union
	{
		/* AOT_DREGISTER */
		struct
		{
			char const * name;
			int64_t offset;
		} dregister;

		/* AOT_DREGISTER2 */
		struct
		{
			char const * name;
			char const * name2;
		} dregister2;

		/* AOT_IMMEDIATE */
		struct
		{
			char const * name;		/* optional */
			uint64_t value;
			int negative;
		} immediate;

		/* AOT_REGISTER */
		struct
		{
			char const * name;
		} _register;
		/* FIXME complete */
	} value;
} ArchOperand;

typedef struct _ArchInstruction
{
	char const * name;
	uint32_t opcode;
	ArchOperandDefinition flags;
	ArchOperandDefinition op1;
	ArchOperandDefinition op2;
	ArchOperandDefinition op3;
} ArchInstruction;

typedef struct _ArchInstructionCall
{
	char const * name;
	ArchOperand operands[3];
	uint32_t operands_cnt;

	/* meta information */
	off_t base;
	size_t offset;
	size_t size;
} ArchInstructionCall;

typedef struct _ArchRegister
{
	char const * name;
	uint32_t size;
	uint32_t id;
} ArchRegister;

typedef struct _ArchPluginHelper
{
	Arch * arch;

	/* callbacks */
	/* accessors */
	char const * (*get_filename)(Arch * arch);
	AsmFunction * (*get_function_by_id)(Arch * arch, AsmId id);
	ArchInstruction * (*get_instruction_by_opcode)(Arch * arch,
			uint8_t size, uint32_t opcode);
	ArchRegister * (*get_register_by_id_size)(Arch * arch, uint32_t id,
			uint32_t size);
	ArchRegister * (*get_register_by_name_size)(Arch * arch,
			char const * name, uint32_t size);
	AsmString * (*get_string_by_id)(Arch * arch, AsmId id);

	/* assembly */
	ssize_t (*write)(Arch * arch, void const * buf, size_t size);

	/* disassembly */
	ssize_t (*peek)(Arch * arch, void * buf, size_t size);
	ssize_t (*read)(Arch * arch, void * buf, size_t size);
	off_t (*seek)(Arch * arch, off_t offset, int whence);
} ArchPluginHelper;

typedef struct _ArchPlugin ArchPlugin;

struct _ArchPlugin
{
	ArchPluginHelper * helper;

	char const * name;

	ArchDescription * description;
	ArchRegister * registers;
	ArchInstruction * instructions;

	int (*init)(ArchPlugin * arch);
	void (*exit)(ArchPlugin * arch);
	int (*write)(ArchPlugin * arch, ArchInstruction * instruction,
			ArchInstructionCall * call);
	int (*decode)(ArchPlugin * arch, ArchInstructionCall * call);
};

#endif /* !DEVEL_ASM_ARCH_H */
