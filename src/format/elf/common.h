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

# if defined(__APPLE__)
#  include "elf_netbsd.h"
# elif defined(__OpenBSD__)
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

# if defined(ELFSIZE) && (ELFSIZE == 32)
#  ifndef ELFCLASS
#   define ELFCLASS	ELFCLASS32
#  endif
#  ifndef Elf_Addr
#   define Elf_Addr	Elf32_Addr
#  endif
#  ifndef Elf_Dyn
#   define Elf_Dyn	Elf32_Dyn
#  endif
#  ifndef Elf_Ehdr
#   define Elf_Ehdr	Elf32_Ehdr
#  endif
#  ifndef Elf_Half
#   define Elf_Half	Elf32_Half
#  endif
#  ifndef Elf_Off
#   define Elf_Off	Elf32_Off
#  endif
#  ifndef Elf_Nhdr
#   define Elf_Nhdr	Elf32_Nhdr
#  endif
#  ifndef Elf_Phdr
#   define Elf_Phdr	Elf32_Phdr
#  endif
#  ifndef Elf_Rel
#   define Elf_Rel	Elf32_Rel
#  endif
#  ifndef Elf_Rela
#   define Elf_Rela	Elf32_Rela
#  endif
#  ifndef Elf_Shdr
#   define Elf_Shdr	Elf32_Shdr
#  endif
#  ifndef Elf_SOff
#   define Elf_SOff	Elf32_SOff
#  endif
#  ifndef Elf_Sword
#   define Elf_Sword	Elf32_Sword
#  endif
#  ifndef Elf_Sym
#   define Elf_Sym	Elf32_Sym
#  endif
#  ifndef Elf_Verdaux
#   define Elf_Verdaux	Elf32_Verdaux
#  endif
#  ifndef Elf_Verdef
#   define Elf_Verdef	Elf32_Verdef
#  endif
#  ifndef Elf_Vernaux
#   define Elf_Vernaux	Elf32_Vernaux
#  endif
#  ifndef Elf_Verneed
#   define Elf_Verneed	Elf32_Verneed
#  endif
#  ifndef Elf_Versym
#   define Elf_Versym	Elf32_Versym
#  endif
#  ifndef Elf_Word
#   define Elf_Word	Elf32_Word
#  endif
# elif defined(ELFSIZE) && (ELFSIZE == 64)
#  ifndef ELFCLASS
#   define ELFCLASS	ELFCLASS64
#  endif
#  ifndef Elf_Addr
#   define Elf_Addr	Elf64_Addr
#  endif
#  ifndef Elf_Dyn
#   define Elf_Dyn	Elf64_Dyn
#  endif
#  ifndef Elf_Ehdr
#   define Elf_Ehdr	Elf64_Ehdr
#  endif
#  ifndef Elf_Half
#   define Elf_Half	Elf64_Half
#  endif
#  ifndef Elf_Off
#   define Elf_Off	Elf64_Off
#  endif
#  ifndef Elf_Nhdr
#   define Elf_Nhdr	Elf64_Nhdr
#  endif
#  ifndef Elf_Phdr
#   define Elf_Phdr	Elf64_Phdr
#  endif
#  ifndef Elf_Rel
#   define Elf_Rel	Elf64_Rel
#  endif
#  ifndef Elf_Rela
#   define Elf_Rela	Elf64_Rela
#  endif
#  ifndef Elf_Shdr
#   define Elf_Shdr	Elf64_Shdr
#  endif
#  ifndef Elf_SOff
#   define Elf_SOff	Elf64_SOff
#  endif
#  ifndef Elf_Sword
#   define Elf_Sword	Elf64_Sword
#  endif
#  ifndef Elf_Sym
#   define Elf_Sym	Elf64_Sym
#  endif
#  ifndef Elf_Verdaux
#   define Elf_Verdaux	Elf64_Verdaux
#  endif
#  ifndef Elf_Verdef
#   define Elf_Verdef	Elf64_Verdef
#  endif
#  ifndef Elf_Vernaux
#   define Elf_Vernaux	Elf64_Vernaux
#  endif
#  ifndef Elf_Verneed
#   define Elf_Verneed	Elf64_Verneed
#  endif
#  ifndef Elf_Versym
#   define Elf_Versym	Elf64_Versym
#  endif
#  ifndef Elf_Word
#   define Elf_Word	Elf64_Word
#  endif
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

#endif /* !ASM_FORMAT_ELF_COMMON_H */
