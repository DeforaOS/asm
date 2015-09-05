/* $Id$ */
/* Copyright (c) 2011-2013 Pierre Pronchery <khorben@defora.org> */
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



#ifndef DEVEL_ASM_ASM_H
# define DEVEL_ASM_ASM_H

# include "code.h"


/* Asm */
/* types */
typedef struct _Asm Asm;

typedef struct _AsmPrefs
{
	char ** defines;
	size_t defines_cnt;
} AsmPrefs;

typedef enum _AsmPluginType { APT_ARCH = 0, APT_FORMAT } AsmPluginType;


/* functions */
Asm * asm_new(char const * arch, char const * format);
void asm_delete(Asm * a);


/* accessors */
/* detection */
char const * asm_get_arch(Asm * a);
int asm_set_arch(Asm * a, char const * arch);

char const * asm_get_format(Asm * a);
int asm_set_format(Asm * a, char const * format);

/* functions */
AsmFunction * asm_set_function(Asm * a, char const * name, off_t offset,
		ssize_t size);

/* sections */
AsmSection * asm_set_section(Asm * a, unsigned int flags, char const * name,
		off_t offset, ssize_t size, off_t base);


/* useful */
/* detection */
int asm_guess_arch(Asm * a);
int asm_guess_format(Asm * a);

/* common */
int asm_close(Asm * a);

/* assemble */
int asm_assemble(Asm * a, AsmPrefs * prefs, char const * infile,
		char const * outfile);
int asm_assemble_string(Asm * a, AsmPrefs * prefs, char const * outfile,
		char const * string);
int asm_open_assemble(Asm * a, char const * outfile);

int asm_instruction(Asm * a, char const * name, unsigned int operands_cnt, ...);

/* deassemble */
AsmCode * asm_deassemble(Asm * a, char const * buffer, size_t size,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);
AsmCode * asm_open_deassemble(Asm * a, char const * filename, int raw);

/* plug-in helpers */
int asm_plugin_list(AsmPluginType type, int decode);

#endif /* !DEVEL_ASM_COMMON_H */
