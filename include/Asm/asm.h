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

# include <sys/types.h>


/* Asm */
/* types */
typedef struct _Asm Asm;

typedef struct _AsmPrefs
{
	char ** defines;
	size_t defines_cnt;
} AsmPrefs;

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
AsmFunction * asm_get_function_by_name(Asm * a, char const * name);
int asm_set_function(Asm * a, char const * name, off_t offset, ssize_t size);

/* labels */
AsmLabel * asm_get_label_by_name(Asm * a, char const * label);
AsmLabel * asm_get_label_by_offset(Asm * a, off_t offset);
int asm_set_label(Asm * a, char const * label, off_t offset);

/* sections */
int asm_set_section(Asm * a, char const * name, off_t offset, ssize_t size);

/* strings */
AsmString * asm_get_string_by_id(Asm * a, AsmId id);
AsmString * asm_get_string_by_name(Asm * a, char const * name);
int asm_set_string(Asm * a, int id, char const * name, off_t offset,
		ssize_t length);


/* useful */
/* detection */
int asm_guess_arch(Asm * a);
int asm_guess_format(Asm * a);

/* common */
int asm_close(Asm * a);

/* assemble */
int asm_assemble(Asm * a, AsmPrefs * prefs, char const * infile,
		char const * outfile);
int asm_open_assemble(Asm * a, char const * outfile);

int asm_instruction(Asm * a, char const * name, unsigned int operands_cnt, ...);

/* deassemble */
int asm_deassemble(Asm * a, char const * buffer, size_t size);
int asm_open_deassemble(Asm * a, char const * filename);

/* plug-in helpers */
int asm_plugin_list(AsmPluginType type);

#endif /* !DEVEL_ASM_AS_H */
