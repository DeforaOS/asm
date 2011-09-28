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



#ifndef ASM_CODE_H
# define ASM_CODE_H

# include <stdio.h>
# include "Asm/arch.h"


/* types */
typedef struct _Code Code;


/* functions */
Code * code_new(char const * arch, char const * format);
Code * code_new_file(char const * arch, char const * format,
		char const * filename);
int code_delete(Code * code);

/* accessors */
char const * code_get_arch(Code * code);
char const * code_get_filename(Code * code);
char const * code_get_format(Code * code);

AsmFunction * code_get_function_by_id(Code * code, AsmId id);
AsmString * code_get_string_by_id(Code * code, AsmId id);

int code_set_function(Code * code, int id, char const * name, off_t offset,
		ssize_t size);
int code_set_string(Code * code, int id, char const * name, off_t offset,
		ssize_t length);

/* useful */
/* common */
int code_open(Code * code, char const * filename);
int code_close(Code * code);

/* assembly */
int code_function(Code * code, char const * function);
int code_instruction(Code * code, ArchInstructionCall * call);
int code_section(Code * code, char const * section);

/* disassembly */
int code_decode(Code * code, int raw);
int code_decode_at(Code * code, char const * section, off_t offset,
		size_t size, off_t base);
int code_decode_buffer(Code * code, char const * buffer, size_t size);
int code_print(Code * code, ArchDescription * description,
		ArchInstructionCall * call);

#endif /* !ASM_CODE_H */
