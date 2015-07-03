/* $Id$ */
/* Copyright (c) 2015 Pierre Pronchery <khorben@defora.org> */
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


#ifndef ASM_FORMAT_ELF_FORMAT_H
# define ASM_FORMAT_ELF_FORMAT_H

# include "common.h"


/* functions */
/* ELF32 */
int elf32_init(AsmFormatPlugin * format);
int elf32_destroy(AsmFormatPlugin * format);
int elf32_section(AsmFormatPlugin * format, char const * name);
char const * elf32_detect(AsmFormatPlugin * format, Elf32_Ehdr * ehdr);
int elf32_decode(AsmFormatPlugin * format, int raw);

/* ELF64 */
int elf64_init(AsmFormatPlugin * format);
int elf64_destroy(AsmFormatPlugin * format);
int elf64_section(AsmFormatPlugin * format, char const * name);
char const * elf64_detect(AsmFormatPlugin * format, Elf64_Ehdr * ehdr);
int elf64_decode(AsmFormatPlugin * format, int raw);

#endif
