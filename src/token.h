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



#ifndef ASM_TOKEN_H
# define ASM_TOKEN_H


/* protected */
/* variables */
extern TokenCode TS_ADDRESS[];
extern TokenCode TS_FUNCTION[];
extern TokenCode TS_IMMEDIATE[];
extern TokenCode TS_INSTRUCTION[];
extern TokenCode TS_NEWLINE[];
extern TokenCode TS_OFFSET[];
extern TokenCode TS_OPERAND_LIST[];
extern TokenCode TS_REGISTER[];
extern TokenCode TS_SECTION[];
extern TokenCode TS_SECTION_NAME[];
extern TokenCode TS_SIGN[];
extern TokenCode TS_SPACE[];
extern TokenCode TS_STATEMENT[];
extern TokenCode TS_SYMBOL[];
extern TokenCode TS_VALUE[];

#endif /* !ASM_TOKEN_H */
