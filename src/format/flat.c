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
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "Asm/format.h"


/* Flat */
/* private */
/* prototypes */
/* plug-in */
static int _flat_disas(FormatPlugin * format, int (*callback)(
			FormatPlugin * format, char const * section,
			off_t offset, size_t size, off_t base));


/* public */
/* variables */
FormatPlugin format_plugin =
{
	NULL,
	"flat",
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	_flat_disas,
	NULL
};


/* private */
/* functions */
/* plug-in */
/* flat_disas */
static int _flat_disas(FormatPlugin * format, int (*callback)(
			FormatPlugin * format, char const * section,
			off_t offset, size_t size, off_t base))
{
	struct stat st;

	if(fstat(fileno(format->helper->fp), &st) != 0)
		return -error_set_code(1, "%s: %s", format->helper->filename,
				strerror(errno));
	return callback(format, NULL, 0, st.st_size, 0);
}
