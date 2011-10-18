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
typedef unsigned int AsmId;

typedef struct _AsmFunction
{
	AsmId id;
	char const * name;
	off_t offset;
	ssize_t size;
} AsmFunction;

typedef struct _AsmLabel
{
	char const * name;
	off_t offset;
} AsmLabel;

typedef struct _AsmString
{
	int id;
	char const * name;
	off_t offset;
	ssize_t length;
} AsmString;

#endif /* !DEVEL_ASM_AS_H */
