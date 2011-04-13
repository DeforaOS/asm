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

# include <stdint.h>


/* AsArch */
/* types */
typedef enum _ArchEndian
{
	ARCH_ENDIAN_BIG = 0,
	ARCH_ENDIAN_LITTLE,
	ARCH_ENDIAN_BOTH
} ArchEndian;

typedef struct _ArchDescription
{
	ArchEndian endian;
	uint32_t alignment;
	uint32_t instruction_size;	/* 0 if not constant */
} ArchDescription;

/* operands */
typedef uint32_t ArchOperand;
# define AOT_NONE	0x0
# define AOT_CONSTANT	0x1		/* flags | offset |   size |  value */
# define AOT_IMMEDIATE	0x2		/* flags | offset |      0 |   size */
# define AOT_REGISTER	0x3		/* flags | offset |   size |     id */
# define AOT_DREGISTER	0x4		/* flags | offset |  dsize |     id */
# define AOT_DREGISTER2	0x5		/* flags | offset |    did |     id */

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
# define AOF_FILTER	0x1
# define AOF_IMPLICIT	0x1		/* for registers */
# define AOF_SIGNED	0x2		/* for immediate */
# define AOF_SOFFSET	0x4
# define AOF_OFFSETSIZE	0x8		/* for registers */

/* macros */
# define AO_GET_FLAGS(operand)	((operand & AOM_FLAGS) >> AOD_FLAGS)
# define AO_GET_OFFSET(operand)	((operand & AOM_OFFSET) >> AOD_OFFSET)
# define AO_GET_SIZE(operand)	((operand & AOM_SIZE) >> AOD_SIZE)
# define AO_GET_TYPE(operand)	((operand & AOM_TYPE) >> AOD_TYPE)
# define AO_GET_VALUE(operand)	((operand & AOM_VALUE) >> AOD_VALUE)

# define AO_IMMEDIATE(flags, offset, size)	((AOT_IMMEDIATE << AOD_TYPE) \
		| (flags << AOD_FLAGS) | (offset << AOD_OFFSET) \
		| (size << AOD_SIZE))
# define AO_REGISTER(flags, offset, size, id)	((AOT_REGISTER << AOD_TYPE) \
		| (flags << AOD_FLAGS) | (offset << AOD_OFFSET) \
		| (size << AOD_SIZE) | (id << AOD_VALUE))
# define AO_DREGISTER(flags, offset, dsize)	((AOT_DREGISTER << AOD_TYPE) \
		| (flags << AOD_FLAGS) | (offset << AOD_OFFSET) \
		| (dsize << AOD_SIZE))
# define AO_DREGISTER2(flags, offset)	((AOT_DREGISTER2 << AOD_TYPE) \
		| (flags << AOD_FLAGS) | (offset << AOD_OFFSET))

typedef struct _ArchInstruction
{
	char * name;
	uint32_t value;
	ArchOperand opcode;
	ArchOperand op1;
	ArchOperand op2;
	ArchOperand op3;
} ArchInstruction;

typedef struct _ArchRegister
{
	char * name;
	uint32_t size;
	uint32_t id;
} ArchRegister;

typedef struct _ArchPlugin ArchPlugin;

struct _ArchPlugin
{
	char const * name;
	char const * format;				/* default format */
	ArchDescription * description;
	ArchRegister * registers;
	ArchInstruction * instructions;
	int (*filter)(ArchPlugin * arch, ArchInstruction * instruction);
};

#endif /* !DEVEL_ASM_ARCH_H */
