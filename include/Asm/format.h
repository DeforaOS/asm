/* $Id$ */
/* Copyright (c) 2011-2015 Pierre Pronchery <khorben@defora.org> */
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



#ifndef DEVEL_ASM_FORMAT_H
# define DEVEL_ASM_FORMAT_H

# include <stdio.h>
# include <System/license.h>
# include "asm.h"


/* AsmFormat */
/* types */
typedef struct _AsmFormat AsmFormat;

typedef struct _AsmFormatPlugin AsmFormatPlugin;

typedef struct _AsmFormatPluginHelper
{
	AsmFormat * format;

	/* callbacks */
	/* accessors */
	char const * (*get_filename)(AsmFormat * format);
	void (*get_functions)(AsmFormat * format, AsmFunction ** functions,
			size_t * functions_cnt);

	/* useful */
	ssize_t (*read)(AsmFormat * format, void * buf, size_t size);
	off_t (*seek)(AsmFormat * format, off_t offset, int whence);

	/* assembly */
	ssize_t (*write)(AsmFormat * format, void const * buf, size_t size);

	/* disassembly */
	/* FIXME let a different architecture be specified in the callback? */
	AsmFunction * (*get_function_by_id)(AsmFormat * format,
			AsmFunctionId id);
	AsmSection * (*get_section_by_id)(AsmFormat * format, AsmSectionId id);
	AsmString * (*get_string_by_id)(AsmFormat * format, AsmStringId id);
	AsmSection * (*set_function)(AsmFormat * format, int id,
			char const * name, off_t offset, ssize_t size);
	AsmSection * (*set_section)(AsmFormat * format, int id,
			unsigned int flags, char const * name, off_t offset,
			ssize_t size, off_t base);
	AsmString * (*set_string)(AsmFormat * format, int id, char const * name,
			off_t offset, ssize_t size);
	int (*decode)(AsmFormat * format, off_t offset, size_t size, off_t base,
			AsmArchInstructionCall ** calls, size_t * calls_cnt);
} AsmFormatPluginHelper;

typedef struct _AsmFormatPluginDefinition
{
	char const * name;
	char const * description;
	LicenseFlags license;

	char const * signature;
	size_t signature_len;

	AsmFormatPlugin * (*init)(AsmFormatPluginHelper * helper,
			char const * arch);
	void (*destroy)(AsmFormatPlugin * format);
	int (*function)(AsmFormatPlugin * format, char const * function);
	int (*section)(AsmFormatPlugin * format, char const * section);

	char const * (*detect)(AsmFormatPlugin * format);
	int (*decode)(AsmFormatPlugin * format, int raw);
	int (*decode_section)(AsmFormatPlugin * format, AsmSection * section,
			AsmArchInstructionCall ** calls, size_t * calls_cnt);
} AsmFormatPluginDefinition;

#endif /* !DEVEL_ASM_FORMAT_H */
