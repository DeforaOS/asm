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



#ifndef DEVEL_ASM_FORMAT_H
# define DEVEL_ASM_FORMAT_H

# include <stdio.h>
# include "asm.h"


/* AsmFormat */
/* types */
typedef struct _Format Format;

typedef struct _FormatPlugin FormatPlugin;

typedef struct _FormatPluginHelper
{
	Format * format;

	/* callbacks */
	/* accessors */
	char const * (*get_filename)(Format * format);

	/* useful */
	ssize_t (*read)(Format * format, void * buf, size_t size);
	off_t (*seek)(Format * format, off_t offset, int whence);

	/* assembly */
	ssize_t (*write)(Format * format, void const * buf, size_t size);

	/* disassembly */
	/* FIXME let a different architecture be specified in the callback */
	AsmString * (*get_string_by_id)(Format * format, AsmId id);
	int (*set_function)(Format * format, int id, char const * name,
			off_t offset, ssize_t size);
	int (*set_string)(Format * format, int id, char const * name,
			off_t offset, ssize_t size);
	int (*decode)(Format * format, char const * section,
			off_t offset, size_t size, off_t base);
} FormatPluginHelper;

struct _FormatPlugin
{
	FormatPluginHelper * helper;

	char const * name;

	char const * signature;
	size_t signature_len;

	int (*init)(FormatPlugin * format, char const * arch);
	int (*exit)(FormatPlugin * format);
	int (*function)(FormatPlugin * format, char const * function);
	int (*section)(FormatPlugin * format, char const * section);

	char const * (*detect)(FormatPlugin * format);
	int (*decode)(FormatPlugin * format);

	void * priv;
};

#endif /* !DEVEL_ASM_FORMAT_H */
