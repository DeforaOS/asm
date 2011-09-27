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



#include <System.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm.h"


/* Java */
/* private */
/* variables */
static ArchDescription _java_description = { "java", ARCH_ENDIAN_BIG, 1, 0 };

static ArchRegister _java_registers[] =
{
	{ NULL,		0, 0 }
};

#define OP1F	(8 << AOD_SIZE)
#define OP_U8	AO_IMMEDIATE(0, 8, 0)
#define OP_U16	AO_IMMEDIATE(0, 16, 0)
#define OP_U32	AO_IMMEDIATE(0, 32, 0)
static ArchInstruction _java_instructions[] =
{
	{ "aaload",	0x32,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "aastore",	0x53,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "aconst_null",0x01,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "aload",	0x19,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "aload_0",	0x2a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "aload_1",	0x2b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "aload_2",	0x2c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "aload_3",	0x2d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "areturn",	0xb0,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "arraylength",0xbe,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "astore",	0x3a,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "astore_0",	0x4b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "astore_1",	0x4c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "astore_2",	0x4d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "astore_3",	0x4e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "athrow",	0xbf,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "baload",	0x33,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "bastore",	0x54,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "bipush",	0x10,	OP1F, OP_U32,     AOT_NONE,  AOT_NONE	},
	{ "caload",	0x34,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "castore",	0x55,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "checkcast",	0xc0,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "d2f",	0x90,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "d2i",	0x8e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "d2l",	0x8f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dadd",	0x63,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "daload",	0x31,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dastore",	0x52,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dcmpg",	0x98,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dcmpl",	0x97,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dconst_0",	0x0e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dconst_1",	0x0f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ddiv",	0x6f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dload",	0x18,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "dload_0",	0x26,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dload_1",	0x27,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dload_2",	0x28,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dload_3",	0x29,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dmul",	0x6b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dneg",	0x77,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "drem",	0x73,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dreturn",	0xaf,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dstore",	0x39,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "dstore_0",	0x47,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dstore_1",	0x48,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dstore_2",	0x49,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dstore_3",	0x4a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dsub",	0x67,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dup",	0x59,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dup_x1",	0x5a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dup_x2",	0x5b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dup2",	0x5c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dup2_x1",	0x5d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "dup2_x2",	0x5e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "f2d",	0x8d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "f2i",	0x8b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "f2l",	0x8c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fadd",	0x62,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "faload",	0x30,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fastore",	0x51,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fcmpg",	0x96,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fcmpl",	0x95,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fconst_0",	0x0b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fconst_1",	0x0c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fconst_2",	0x0d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fdiv",	0x6e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fload",	0x17,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "fload_0",	0x22,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fload_1",	0x23,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fload_2",	0x24,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fload_3",	0x25,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fmul",	0x6a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fneg",	0x76,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "frem",	0x72,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "freturn",	0xae,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fstore_0",	0x43,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fstore_1",	0x44,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fstore_2",	0x45,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fstore_3",	0x46,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "fsub",	0x66,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "getfield",	0xb4,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "getstatic",	0xb2,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "goto",	0xa7,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "goto_w",	0xc8,	OP1F, OP_U32,     AOT_NONE,  AOT_NONE	},
	{ "i2b",	0x91,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "i2c",	0x92,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "i2d",	0x87,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "i2f",	0x86,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "i2l",	0x85,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "i2s",	0x93,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iadd",	0x60,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iaload",	0x2e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iand",	0x7e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iastore",	0x4f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_m1",	0x02,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_0",	0x03,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_1",	0x04,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_2",	0x05,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_3",	0x06,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_4",	0x07,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iconst_5",	0x08,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "idiv",	0x6c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "if_acmpeq",	0xa5,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_acmpne",	0xa6,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_icmpeq",	0x9f,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_icmpne",	0xa0,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_icmplt",	0xa1,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_icmpge",	0xa2,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_icmpgt",	0xa3,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "if_icmple",	0xa4,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifeq",	0x99,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifne",	0x9a,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "iflt",	0x9b,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifge",	0x9c,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifgt",	0x9d,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifle",	0x9e,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifnonnull",	0xc7,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ifnull",	0xc6,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "iinc",	0x84,	OP1F, OP_U8,      OP_U8,     AOT_NONE	},
	{ "iload",	0x15,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "iload_0",	0x1a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iload_1",	0x1b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iload_2",	0x1c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iload_3",	0x1d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "impdep1",	0xfe,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "impdep2",	0xff,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "imul",	0x68,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ineg",	0x74,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "instanceof",	0xc1,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "invokeinterface",0xb9,OP1F,OP_U16,     OP_U8,     AOT_NONE	},
	{ "invokespecial",0xb7,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "invokestatic",0xb8,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "invokevirtual",0xb6,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ior",	0x80,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "irem",	0x70,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ireturn",	0xac,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ishl",	0x78,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ishr",	0x7a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "istore",	0x36,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "istore_0",	0x3b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "istore_0",	0x3c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "istore_0",	0x3d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "istore_3",	0x3e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "isub",	0x64,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "iushr",	0x7c,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ixor",	0x82,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "jsr",	0xa8,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "jsr_w",	0xc9,	OP1F, OP_U32,     AOT_NONE,  AOT_NONE	},
	{ "l2d",	0x8a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "l2f",	0x89,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "l2i",	0x88,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ladd",	0x61,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "laload",	0x2f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "land",	0x7f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lastore",	0x50,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lcmp",	0x94,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lconst_0",	0x09,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lconst_1",	0x0a,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "ldc",	0x12,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "ldc_w",	0x13,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ldc2_w",	0x14,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ldiv",	0x6d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lload",	0x16,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "lload_0",	0x1e,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lload_1",	0x1f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lload_2",	0x20,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lload_3",	0x21,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lmul",	0x69,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lneg",	0x75,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lor",	0x81,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lrem",	0x71,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lreturn",	0xad,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lookupswitch",0xab,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lshl",	0x79,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lshr",	0x7b,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lstore",	0x37,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "lstore_0",	0x3f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lstore_1",	0x40,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lstore_2",	0x41,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lstore_3",	0x42,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lsub",	0x65,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lushr",	0x7d,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "lxor",	0x83,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "monitorenter",0xc2,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "monitorexit",0xc3,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "multianewarray",0xc5,OP1F, OP_U16,     OP_U8,     AOT_NONE	},
	{ "new",	0xbb,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "newarray",	0xbb,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "nop",	0x00,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "pop",	0x57,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "pop2",	0x58,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "putfield",	0xb5,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "putstatic",	0xb3,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "ret",	0xa9,	OP1F, OP_U8,      AOT_NONE,  AOT_NONE	},
	{ "return",	0xb1,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "saload",	0x35,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "sastore",	0x56,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "sipush",	0x11,	OP1F, OP_U16,     AOT_NONE,  AOT_NONE	},
	{ "swap",	0x5f,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "tableswitch",0xaa,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
	{ "wide",	0xc4,	OP1F, OP_U8,      OP_U16,    AOT_NONE	},
	{ "wide",	0xc4,	OP1F, OP_U8,      OP_U8,     OP_U16	},
	{ "xxxunusedxxx",0xba,	OP1F, AOT_NONE,   AOT_NONE,  AOT_NONE	},
#include "common.ins"
#include "null.ins"
};


/* prototypes */
/* plug-in */
static int _java_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);
static int _java_decode(ArchPlugin * plugin, ArchInstructionCall * call);


/* public */
/* variables */
ArchPlugin arch_plugin =
{
	NULL,
	"java",
	&_java_description,
	_java_registers,
	_java_instructions,
	NULL,
	NULL,
	_java_write,
	_java_decode
};


/* private */
/* functions */
/* plug-in */
static int _java_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	size_t i;
	ArchOperandDefinition definitions[3];
	ArchOperand * ao;
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

	if((helper->write(helper->arch, &instruction->opcode, 1)) != 1)
		return -1;
	definitions[0] = instruction->op1;
	definitions[1] = instruction->op2;
	definitions[2] = instruction->op3;
	for(i = 0; i < call->operands_cnt; i++)
	{
		ao = &call->operands[i];
		if(AO_GET_TYPE(ao->definition) != AOT_IMMEDIATE)
			return -error_set_code(1, "%s", "Not implemented");
		if(AO_GET_SIZE(definitions[i]) == 8)
		{
			u8 = ao->value.immediate.value;
			if(helper->write(helper->arch, &u8, 1) != 1)
				return -1;
		}
		else if(AO_GET_SIZE(definitions[i]) == 16)
		{
			u16 = _htob16(ao->value.immediate.value);
			if(helper->write(helper->arch, &u16, 2) != 2)
				return -1;
		}
		else if(AO_GET_SIZE(definitions[i]) == 32)
		{
			u32 = _htob32(ao->value.immediate.value);
			if(helper->write(helper->arch, &u32, 4) != 4)
				return -1;
		}
		else
			return -error_set_code(1, "%s", "Size not implemented");
	}
	return 0;
}


/* java_decode */
static int _java_decode(ArchPlugin * plugin, ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint8_t u8;
	ArchInstruction * ai;
	size_t i;
	ArchOperand * ao;
	uint16_t u16;
	uint32_t u32;

	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 8, u8))
			== NULL)
	{
		call->name = "db";
		call->operands[0].definition = AO_IMMEDIATE(0, 8, 0);
		call->operands[0].value.immediate.name = NULL;
		call->operands[0].value.immediate.value = u8;
		call->operands[0].value.immediate.negative = 0;
		call->operands_cnt = 1;
		return 0;
	}
	call->name = ai->name;
	call->operands[0].definition = ai->op1;
	call->operands[1].definition = ai->op2;
	call->operands[2].definition = ai->op3;
	for(i = 0; i < 3 && AO_GET_TYPE(call->operands[i].definition)
			!= AOT_NONE; i++)
	{
		ao = &call->operands[i];
		if(AO_GET_TYPE(ao->definition) != AOT_IMMEDIATE)
			/* XXX should there be more types? */
			return -error_set_code(1, "%s", "Not implemented");
		if(AO_GET_SIZE(ao->definition) == 8)
		{
			if(helper->read(helper->arch, &u8, 1) != 1)
				return -1;
			ao->value.immediate.value = u8;
		}
		else if(AO_GET_SIZE(ao->definition) == 16)
		{
			if(helper->read(helper->arch, &u16, 2) != 2)
				return -1;
			u16 = _htob16(u16);
			ao->value.immediate.value = u16;
		}
		else if(AO_GET_SIZE(ao->definition) == 32)
		{
			if(helper->read(helper->arch, &u32, 4) != 4)
				return -1;
			u32 = _htob32(u32);
			ao->value.immediate.value = u32;
		}
		else
			return -error_set_code(1, "%s", "Size not implemented");
		ao->value.immediate.name = NULL;
		ao->value.immediate.negative = 0;
	}
	call->operands_cnt = i;
	return 0;
}
