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



#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <System.h>
#include "common.h"


/* public */
/* constants */
const ElfSectionValues elf_section_values[] =
{
	{ ".bss",	SHT_NOBITS,	SHF_ALLOC | SHF_WRITE		},
	{ ".comment",	SHT_PROGBITS,	0				},
	{ ".data",	SHT_PROGBITS,	SHF_ALLOC | SHF_WRITE		},
	{ ".data1",	SHT_PROGBITS,	SHF_ALLOC | SHF_WRITE		},
	{ ".debug",	SHT_PROGBITS,	0				},
	{ ".dynamic",	SHT_DYNAMIC,	0				},
	{ ".dynstr",	SHT_STRTAB,	SHF_ALLOC			},
	{ ".dynsym",	SHT_DYNSYM,	SHF_ALLOC			},
	{ ".fini",	SHT_PROGBITS,	SHF_ALLOC | SHF_EXECINSTR	},
	{ ".got",	SHT_PROGBITS,	0				},
	{ ".hash",	SHT_HASH,	SHF_ALLOC			},
	{ ".init",	SHT_PROGBITS,	SHF_ALLOC | SHF_EXECINSTR	},
	{ ".interp",	SHT_PROGBITS,	0				},
	{ ".line",	SHT_PROGBITS,	0				},
	{ ".note",	SHT_NOTE,	0				},
	{ ".plt",	SHT_PROGBITS,	0				},
	{ ".rodata",	SHT_PROGBITS,	SHF_ALLOC			},
	{ ".rodata1",	SHT_PROGBITS,	SHF_ALLOC			},
	{ ".shstrtab",	SHT_STRTAB,	0				},
	{ ".strtab",	SHT_STRTAB,	0				},
	{ ".symtab",	SHT_SYMTAB,	0				},
	{ ".text",	SHT_PROGBITS,	SHF_ALLOC | SHF_EXECINSTR	},
	{ NULL,		0,		0				}
};


/* functions */
/* elf_error */
int elf_error(AsmFormatPlugin * format)
{
	return -error_set_code(1, "%s: %s", format->helper->get_filename(
				format->helper->format), strerror(errno));
}


/* ElfStrtab */
/* elfstrtab_set */
int elfstrtab_set(AsmFormatPlugin * format, ElfStrtab * strtab,
		char const * name)
{
	size_t len;
	size_t cnt;
	char * p;

	if((len = strlen(name)) == 0 && strtab->cnt != 0)
		return 0;
	if((cnt = strtab->cnt) == 0)
		cnt++;
	if((p = realloc(strtab->buf, sizeof(char) * (cnt + len + 1))) == NULL)
		return -elf_error(format);
	else if(strtab->buf == NULL)
		p[0] = '\0';
	strtab->buf = p;
	if(len == 0)
	{
		strtab->cnt = cnt;
		return 0;
	}
	strtab->cnt = cnt + len + 1;
	memcpy(&strtab->buf[cnt], name, len + 1);
	return cnt;
}
