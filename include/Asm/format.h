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



#ifndef DEVEL_ASM_FORMAT_H
# define DEVEL_ASM_FORMAT_H

# include <stdio.h>


/* AsmFormat */
/* types */
typedef struct _FormatPluginHelper
{
	char const * filename;
	FILE * fp;
	void * priv;
} FormatPluginHelper;

typedef struct _FormatPlugin FormatPlugin;

struct _FormatPlugin
{
	FormatPluginHelper * helper;

	char const * name;

	char const * signature;
	size_t signature_len;

	int (*init)(FormatPlugin * format, char const * arch);
	int (*exit)(FormatPlugin * format);
	int (*function)(FormatPlugin * format, char const * function);
	int (*section)(FormatPlugin * format, char const * section);

	char const * (*detect)(FormatPlugin * format);
	/* FIXME:
	 * - put the callback in the helper structure
	 * - let a different architecture be specified in the callback */
	int (*disas)(FormatPlugin * format, int (*callback)(
				FormatPlugin * format, char const * section,
				off_t offset, size_t size, off_t base));

	void * priv;
};

#endif /* !DEVEL_ASM_FORMAT_H */
