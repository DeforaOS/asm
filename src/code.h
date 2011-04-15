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
# include "Asm/asm.h"
# include "arch.h"
# include "format.h"
# include "token.h"


/* types */
typedef struct _Code Code;


/* functions */
Code * code_new(char const * arch, char const * format);
int code_delete(Code * code);

/* accessors */
Arch * code_get_arch(Code * code);
char const * code_get_arch_name(Code * code);
Format * code_get_format(Code * code);
char const * code_get_format_name(Code * code);

/* useful */
int code_open(Code * code, char const * filename);
int code_close(Code * code);

ArchInstruction * code_decode(Code * code, char const * buffer, size_t * size);

int code_function(Code * code, char const * function);
int code_instruction(Code * code, ArchInstructionCall * call);
int code_section(Code * code, char const * section);

#endif /* !ASM_CODE_H */
