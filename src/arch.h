/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel Asm */
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


/* AsmArch */
/* public */
/* functions */
AsmArch * arch_new(char const * name);
void arch_delete(AsmArch * arch);


/* accessors */
int arch_can_decode(AsmArch * arch);

AsmArchDescription * arch_get_description(AsmArch * arch);
char const * arch_get_format(AsmArch * arch);
char const * arch_get_name(AsmArch * arch);

AsmArchInstruction * arch_get_instruction(AsmArch * arch, size_t index);
AsmArchInstruction * arch_get_instruction_by_name(AsmArch * arch, char const * name);
AsmArchInstruction * arch_get_instruction_by_opcode(AsmArch * arch, uint8_t size,
		uint32_t opcode);
AsmArchInstruction * arch_get_instruction_by_call(AsmArch * arch,
		AsmArchInstructionCall * call);

AsmArchRegister * arch_get_register(AsmArch * arch, size_t index);
AsmArchRegister * arch_get_register_by_id_size(AsmArch * arch, uint32_t id,
		uint32_t size);
AsmArchRegister * arch_get_register_by_name(AsmArch * arch, char const * name);
AsmArchRegister * arch_get_register_by_name_size(AsmArch * arch, char const * name,
		uint32_t size);

/* useful */
int arch_init(AsmArch * arch, char const * filename, FILE * fp);
int arch_init_buffer(AsmArch * arch, char const * buffer, size_t size);
int arch_exit(AsmArch * arch);

/* assembly */
int arch_encode(AsmArch * arch, AsmArchInstruction * instruction,
		AsmArchInstructionCall * call);

/* deassembly */
int arch_decode(AsmArch * arch, AsmCode * code, off_t base,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);
int arch_decode_at(AsmArch * arch, AsmCode * code, off_t offset, size_t size,
	off_t base, AsmArchInstructionCall ** calls, size_t * calls_cnt);
ssize_t arch_read(AsmArch * arch, void * buf, size_t cnt);
off_t arch_seek(AsmArch * arch, off_t offset, int whence);

#endif /* !ASM_ARCH_H */
