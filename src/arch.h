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



#ifndef ASM_ARCH_H
# define ASM_ARCH_H

# include <sys/types.h>
# include <stdint.h>
# include "Asm.h"


/* Arch */
/* public */
/* types */
typedef struct _Arch Arch;


/* functions */
Arch * arch_new(char const * name);
void arch_delete(Arch * arch);


/* accessors */
ArchDescription * arch_get_description(Arch * arch);
char const * arch_get_format(Arch * arch);
char const * arch_get_name(Arch * arch);

ArchInstruction * arch_get_instruction(Arch * arch, size_t index);
ArchInstruction * arch_get_instruction_by_name(Arch * arch, char const * name);
ArchInstruction * arch_get_instruction_by_opcode(Arch * arch, uint8_t size,
		uint32_t opcode);
ArchInstruction * arch_get_instruction_by_operands(Arch * arch,
		char const * name, AsOperand ** operands, size_t operands_cnt);

ArchRegister * arch_get_register(Arch * arch, size_t index);
ArchRegister * arch_get_register_by_id(Arch * arch, unsigned int id);
ArchRegister * arch_get_register_by_name(Arch * arch, char const * name);
ArchRegister * arch_get_register_by_name_size(Arch * arch, char const * name,
		uint32_t size);

/* useful */
int arch_filter(Arch * arch, ArchInstruction * ai, unsigned char * buf,
		size_t size);

#endif /* !ASM_ARCH_H */
