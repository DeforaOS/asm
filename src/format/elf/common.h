/* $Id$ */
/* Copyright (c) 2015-2016 Pierre Pronchery <khorben@defora.org> */
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


#ifndef ASM_FORMAT_ELF_COMMON_H
# define ASM_FORMAT_ELF_COMMON_H

# ifdef __OpenBSD__
#  include <elf_abi.h>
# else
#  include <elf.h>
# endif
#  include "Asm.h"

/* portability */
# ifndef Elf64_Quarter
#  define Elf64_Quarter		unsigned char
# endif
# ifndef EM_486
#  define EM_486		6
# endif


/* ELF */
/* types */
typedef struct _AsmFormatPlugin Elf;

typedef struct _ElfArch
{
	char const * arch;
	unsigned char machine;
	unsigned char capacity;
	unsigned char endian;
	uint64_t addralign;
} ElfArch;

typedef struct _ElfSectionValues
{
	char const * name;
	Elf32_Word type;	/* works for 64-bit too */
	Elf32_Word flags;
} ElfSectionValues;

typedef struct _ElfStrtab
{
	char * buf;
	size_t cnt;
} ElfStrtab;

struct _AsmFormatPlugin
{
	AsmFormatPluginHelper * helper;

	const ElfArch * arch;
	int (*destroy)(Elf * elf);
	int (*section)(Elf * elf, char const * name);
	int (*decode)(Elf * elf, int raw);

	ElfStrtab shstrtab;			/* section string table */

	/* ELF32 */
	Elf32_Shdr * es32;
	int es32_cnt;

	/* ELF64 */
	Elf64_Shdr * es64;
	int es64_cnt;
};


/* constants */
extern const ElfArch elf_arch[];
extern const ElfArch * elf_arch_native;

extern const ElfSectionValues elf_section_values[];


/* functions */
int elf_error(AsmFormatPlugin * format);

/* ElfStrtab */
int elfstrtab_set(AsmFormatPlugin * format, ElfStrtab * strtab,
		char const * name);

#endif
