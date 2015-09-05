/* $Id$ */
/* Copyright (c) 2011-2015 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel asm */
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



#ifndef DEVEL_ASM_COMMON_H
# define DEVEL_ASM_COMMON_H

# include <sys/types.h>


/* common */
/* types */
typedef int AsmElementId;

typedef struct _AsmElement
{
	AsmElementId id;
	unsigned int flags;
	char * name;
	off_t offset;
	ssize_t size;
	off_t base;
} AsmElement;

typedef enum _AsmElementType
{
	AET_FUNCTION = 0,
	AET_LABEL,
	AET_SECTION,
	AET_STRING
} AsmElementType;
# define AET_LAST AET_STRING
# define AET_COUNT (AET_LAST + 1)

typedef AsmElementId AsmFunctionId;
typedef struct _AsmElement AsmFunction;

typedef AsmElementId AsmLabelId;
typedef struct _AsmElement AsmLabel;

typedef AsmElementId AsmSectionId;
typedef struct _AsmElement AsmSection;

typedef AsmElementId AsmStringId;
typedef struct _AsmElement AsmString;

/* arch */
typedef uint32_t AsmArchOperandDefinition;

typedef struct _AsmArchInstruction
{
	char const * name;
	uint32_t opcode;
	AsmArchOperandDefinition flags;
	AsmArchOperandDefinition op1;
	AsmArchOperandDefinition op2;
	AsmArchOperandDefinition op3;
	AsmArchOperandDefinition op4;
	AsmArchOperandDefinition op5;
} AsmArchInstruction;

#endif /* !DEVEL_ASM_COMMON_H */
