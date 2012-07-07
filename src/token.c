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



#include "common.h"
#include "token.h"


/* protected */
/* variables */
TokenCode TS_ADDRESS[] = { AS_CODE_OPERATOR_LBRACKET, TC_NULL };
TokenCode TS_FUNCTION[] = { AS_CODE_WORD, TC_NULL };
TokenCode TS_IMMEDIATE[] = { AS_CODE_IMMEDIATE, AS_CODE_OPERATOR_MINUS,
	AS_CODE_OPERATOR_PLUS, TC_NULL };
TokenCode TS_INSTRUCTION[] = { AS_CODE_WORD, TC_NULL };
TokenCode TS_NEWLINE[] = { AS_CODE_WHITESPACE, AS_CODE_NEWLINE, TC_NULL };
TokenCode TS_OFFSET[] = { AS_CODE_OPERATOR_MINUS, AS_CODE_OPERATOR_PLUS,
	TC_NULL };
TokenCode TS_OPERAND_LIST[] = { AS_CODE_WORD, AS_CODE_IMMEDIATE,
	AS_CODE_REGISTER, AS_CODE_OPERATOR_LBRACKET, TC_NULL };
TokenCode TS_REGISTER[] = { AS_CODE_REGISTER, TC_NULL };
TokenCode TS_SECTION[] = { AS_CODE_OPERATOR_DOT, TC_NULL };
TokenCode TS_SECTION_NAME[] = { AS_CODE_WORD, TC_NULL };
TokenCode TS_SIGN[] = { AS_CODE_OPERATOR_MINUS, AS_CODE_OPERATOR_PLUS,
	TC_NULL };
TokenCode TS_SPACE[] = { AS_CODE_WHITESPACE, TC_NULL };
TokenCode TS_STATEMENT[] = { AS_CODE_WORD, AS_CODE_WHITESPACE, AS_CODE_NEWLINE,
	TC_NULL };
TokenCode TS_SYMBOL[] = { AS_CODE_WORD, TC_NULL };
TokenCode TS_VALUE[] = { AS_CODE_IMMEDIATE, AS_CODE_OPERATOR_MINUS,
	AS_CODE_OPERATOR_PLUS, AS_CODE_REGISTER, AS_CODE_WORD, TC_NULL };
