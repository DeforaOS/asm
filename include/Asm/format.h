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

# if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#  include <sys/endian.h>
# elif defined(__linux__)
#  include <endian.h>
# endif
# include <stdio.h>


/* As */
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
	int (*disas)(FormatPlugin * format, int (*callback)(
				FormatPlugin * format, char const * section,
				off_t offset, size_t size, off_t base));

	void * priv;
};


/* helpers */
# if _BYTE_ORDER == _BIG_ENDIAN
#  define _htob16(a) (a) 
#  define _htol16(a) (((a) & 0xff) << 8 | ((a) & 0xff00) >> 8)
#  define _htob32(a) (a) 
#  define _htol32(a) (((a) & 0xff) << 24 | (((a) & 0xff00) << 8) \
		| (((a) & 0xff0000) >> 8) | ((a) & 0xff000000) >> 24)
#  define _htob64(a) (a)
#  define _htol64(a) (((a) & 0xff) << 56) | (((a) & 0xff00) << 40) \
		| (((a) & 0xff0000) << 24) | (((a) & 0xff000000) << 8) \
		| (((a) & 0xff00000000) >> 8) \
		| (((a) & 0xff0000000000) >> 24) \
		| (((a) & 0xff000000000000) >> 40) \
		| (((a) & 0xff00000000000000) >> 56)
# elif _BYTE_ORDER == _LITTLE_ENDIAN
#  define _htob16(a) (((a) & 0xff) << 8 | ((a) & 0xff00) >> 8)
#  define _htol16(a) (a)
#  define _htob32(a) (((a) & 0xff) << 24 | (((a) & 0xff00) << 8) \
		| (((a) & 0xff0000) >> 8) | ((a) & 0xff000000) >> 24)
#  define _htol32(a) (a) 
#  define _htob64(a) (((a) & 0xff) << 56) | (((a) & 0xff00) << 40) \
		| (((a) & 0xff0000) << 24) | (((a) & 0xff000000) << 8) \
		| (((a) & 0xff00000000) >> 8) \
		| (((a) & 0xff0000000000) >> 24) \
		| (((a) & 0xff000000000000) >> 40) \
		| (((a) & 0xff00000000000000) >> 56)
#  define _htol64(a) (a)
# else
#  warning "Could not determine endian on your system"
# endif

#endif /* !DEVEL_ASM_FORMAT_H */
