/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
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



#ifndef ASM_CODE_H
# define ASM_CODE_H

# include <stdio.h>
# include "Asm/code.h"


/* functions */
AsmCode * asmcode_new(char const * arch, char const * format);
AsmCode * asmcode_new_file(char const * arch, char const * format,
		char const * filename);
int asmcode_delete(AsmCode * code);

/* useful */
/* common */
int asmcode_open(AsmCode * code, char const * filename);
int asmcode_close(AsmCode * code);

/* elements */
AsmElement * asmcode_get_element_by_id(AsmCode * a, AsmElementType type,
		AsmElementId id);
AsmElement * asmcode_get_element_by_name(AsmCode * af, AsmElementType type,
		char const * name);
AsmElement * asmcode_get_element_by_offset(AsmCode * af, AsmElementType type,
		off_t offset);
int asmcode_get_elements(AsmCode * af, AsmElementType type,
		AsmElement ** elements, size_t * count);

#endif /* !ASM_CODE_H */
