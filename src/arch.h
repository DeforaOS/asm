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

# include <stdint.h>
# include <stdio.h>
# include "Asm/arch.h"
# include "code.h"


/* Arch */
/* public */
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
ArchInstruction * arch_get_instruction_by_call(Arch * arch,
		ArchInstructionCall * call);

ArchRegister * arch_get_register(Arch * arch, size_t index);
ArchRegister * arch_get_register_by_id_size(Arch * arch, uint32_t id,
		uint32_t size);
ArchRegister * arch_get_register_by_name(Arch * arch, char const * name);
ArchRegister * arch_get_register_by_name_size(Arch * arch, char const * name,
		uint32_t size);

/* useful */
int arch_init(Arch * arch, char const * filename, FILE * fp);
int arch_init_buffer(Arch * arch, char const * buffer, size_t size);
int arch_exit(Arch * arch);

/* assembly */
int arch_write(Arch * arch, ArchInstruction * instruction,
		ArchInstructionCall * call);

/* disassembly */
int arch_decode(Arch * arch, Code * code, ArchInstructionCall ** calls,
		size_t * calls_cnt);
int arch_decode_at(Arch * arch, Code * code, ArchInstructionCall ** calls,
		size_t * calls_cnt, off_t offset, size_t size, off_t base);
ssize_t arch_read(Arch * arch, void * buf, size_t cnt);
off_t arch_seek(Arch * arch, off_t offset, int whence);

#endif /* !ASM_ARCH_H */
