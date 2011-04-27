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



#ifndef DEVEL_ASM_ASM_H
# define DEVEL_ASM_ASM_H

# include <stdio.h>
# include "arch.h"


/* Asm */
/* types */
typedef struct _Asm Asm;

typedef enum _AsmPluginType { APT_ARCH = 0, APT_FORMAT } AsmPluginType;


/* functions */
Asm * asm_new(char const * arch, char const * format);
void asm_delete(Asm * a);


/* accessors */
char const * asm_get_arch_name(Asm * a);
char const * asm_get_format_name(Asm * a);


/* useful */
int asm_decode(Asm * a, char const * buffer, size_t size);
int asm_decode_file(Asm * a, char const * filename, FILE * fp);
int asm_parse(Asm * a, char const * infile, char const * outfile);

int asm_open(Asm * a, char const * outfile);
int asm_close(Asm * a);
int asm_section(Asm * a, char const * name);
int asm_function(Asm * a, char const * name);
int asm_instruction(Asm * a, char const * name, unsigned int operands_cnt, ...);


/* plugins helpers */
int asm_plugin_list(AsmPluginType type);

#endif /* !DEVEL_ASM_AS_H */
