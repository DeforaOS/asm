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



#ifndef ASM_FORMAT_H
# define ASM_FORMAT_H

# include "Asm/format.h"
# include "code.h"


/* Format */
/* public */
/* types */
typedef int (*FormatDecodeCallback)(void * priv, char const * section,
		off_t offset, size_t size, off_t base);
typedef AsmString * (*FormatGetStringByIdCallback)(void * priv, AsmId id);
typedef int (*FormatSetFunctionCallback)(void * priv, int id, char const * name,
		off_t offset, ssize_t length);
typedef int (*FormatSetStringCallback)(void * priv, int id, char const * name,
		off_t offset, ssize_t length);

/* functions */
Format * format_new(char const * format, char const * arch);
void format_delete(Format * format);

/* accessors */
char const * format_get_name(Format * format);

/* useful */
/* assembly */
int format_init(Format * format, char const * filename, FILE * fp);
int format_exit(Format * format);

int format_function(Format * format, char const * function);
int format_section(Format * format, char const * section);

/* disassembly */
int format_decode(Format * format, Code * code);

#endif /* !ASM_FORMAT_H */
