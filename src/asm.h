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



#ifndef ASM_ASM_H
# define ASM_ASM_H

# include "Asm/asm.h"
# include "arch.h"
# include "format.h"


/* protected */
/* functions */
/* accessors */
Arch * asm_get_arch(Asm * a);
Format * asm_get_format(Asm * a);


/* useful */
int asm_open(Asm * a, char const * outfile);
int asm_close(Asm * a);

#endif /* !ASM_ASM_H */
