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



#ifndef DEVEL_ASM_COMMON_H
# define DEVEL_ASM_COMMON_H

# include <sys/types.h>


/* common */
/* types */
typedef int AsmElementId;

typedef struct _AsmElement
{
	AsmElementId id;
	char * name;
	off_t offset;
	ssize_t size;
	off_t base;
} AsmElement;

typedef enum _AsmElementType
{
	AET_FUNCTION,
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

#endif /* !DEVEL_ASM_COMMON_H */
