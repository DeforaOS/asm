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



#ifndef ASM_FORMAT_H
# define ASM_FORMAT_H

# include "Asm/common.h"
# include "Asm/format.h"
# include "code.h"


/* AsmFormat */
/* public */
/* types */
typedef int (*AsmFormatDecodeCallback)(void * priv, char const * section,
		off_t offset, size_t size, off_t base);
typedef AsmString * (*AsmFormatGetStringByIdCallback)(void * priv,
		AsmStringId id);
typedef int (*AsmFormatSetFunctionCallback)(void * priv, AsmFunctionId id,
		char const * name, off_t offset, ssize_t length);
typedef int (*AsmFormatSetStringCallback)(void * priv, AsmStringId id,
		char const * name, off_t offset, ssize_t length);

/* functions */
AsmFormat * format_new(char const * format);
AsmFormat * format_new_match(char const * filename, FILE * fp);
void format_delete(AsmFormat * format);

/* accessors */
int format_can_decode(AsmFormat * format);

char const * format_get_description(AsmFormat * format);
char const * format_get_name(AsmFormat * format);

/* useful */
/* assembly */
int format_init(AsmFormat * format, char const * arch, char const * filename,
		FILE * fp);
int format_exit(AsmFormat * format);

char const * format_guess_arch(AsmFormat * format, char const * hint);

int format_directive(AsmFormat * format, char const * name, char const ** args,
		size_t args_cnt);
int format_function(AsmFormat * format, char const * name);
int format_section(AsmFormat * format, char const * name);

/* disassembly */
int format_decode(AsmFormat * format, AsmCode * code, int raw);
int format_decode_section(AsmFormat * format, AsmCode * code,
		AsmSection * section, AsmArchInstructionCall ** calls,
		size_t * calls_cnt);
char const * format_detect_arch(AsmFormat * format);
int format_match(AsmFormat * format);

#endif /* !ASM_FORMAT_H */
