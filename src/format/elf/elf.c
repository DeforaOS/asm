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
#include "common.h"

#if (ELFSIZE == 64)
# define elf_init		elf64_init
# define elf_section		elf64_section
# define esSZ			es64
# define esSZ_cnt		es64_cnt
#elif (ELFSIZE == 32)
# define elf_init		elf32_init
# define elf_section		elf32_section
# define esSZ			es32
# define esSZ_cnt		es32_cnt
#else
# error	Unsupported ELF size
#endif


/* public */
/* functions */
/* elf_init */
int elf_init(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format;
	const ElfArch * ea = elf->arch;
	Elf_Ehdr hdr;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	memset(&hdr, 0, sizeof(hdr));
	memcpy(&hdr.e_ident, ELFMAG, SELFMAG);
	hdr.e_ident[EI_CLASS] = ELFCLASS;
	hdr.e_ident[EI_DATA] = ea->endian;
	hdr.e_ident[EI_VERSION] = EV_CURRENT;
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.e_type = _htob16(ET_REL);
		hdr.e_machine = _htob16(ea->machine);
		hdr.e_version = _htob32(EV_CURRENT);
		hdr.e_ehsize = _htob16(sizeof(hdr));
		hdr.e_shentsize = _htob16(sizeof(Elf_Shdr));
		hdr.e_shstrndx = _htob16(SHN_UNDEF);
	}
	else
	{
		hdr.e_type = _htol16(ET_REL);
		hdr.e_machine = _htol16(ea->machine);
		hdr.e_version = _htol32(EV_CURRENT);
		hdr.e_ehsize = _htol16(sizeof(hdr));
		hdr.e_shentsize = _htol16(sizeof(Elf_Shdr));
		hdr.e_shstrndx = _htol16(SHN_UNDEF);
	}
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	return 0;
}


/* elf_section */
static ElfSectionValues const * _section_values(char const * name);

int elf_section(AsmFormatPlugin * format, char const * name)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format;
	int ss;
	Elf_Shdr * p;
	ElfSectionValues const * esv;
	long offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	if((ss = elfstrtab_set(format, &elf->shstrtab, name)) < 0)
		return -1;
	if((p = realloc(elf->esSZ, sizeof(*p) * (elf->esSZ_cnt + 1))) == NULL)
		return elf_error(format);
	elf->esSZ = p;
	p = &elf->esSZ[elf->esSZ_cnt++];
	memset(p, 0, sizeof(*p));
	esv = _section_values(name);
	p->sh_name = ss;
	p->sh_type = esv->type;
	p->sh_flags = esv->flags;
	if((offset = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() offset %ld\n", __func__, offset);
#endif
	p->sh_offset = offset;
	p->sh_link = SHN_UNDEF; /* FIXME */
	return 0;
}

/* section_values */
static ElfSectionValues const * _section_values(char const * name)
{
	ElfSectionValues const * esv;
	int cmp;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	for(esv = elf_section_values; esv->name != NULL; esv++)
		if((cmp = strcmp(esv->name, name)) == 0)
			return esv;
		else if(cmp > 0)
			break;
	for(; esv->name != NULL; esv++);
	return esv;
}
