/* $Id$ */
/* Copyright (c) 2011-2017 Pierre Pronchery <khorben@defora.org> */
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
/* FIXME:
 * - ensure the first section output is of type SHN_UNDEF
 * - use set_string() to store and remember strings? */



#include <System.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elf/common.h"
#include "elf/elf.h"


/* ELF */
/* private */
/* prototypes */
/* plug-in */
static AsmFormatPlugin * _elf_init(AsmFormatPluginHelper * helper,
		char const * arch);
static int _elf_destroy(AsmFormatPlugin * format);
static int _elf_section(AsmFormatPlugin * format, char const * name);
static char const * _elf_guess(AsmFormatPlugin * format, char const * hint);
static char const * _elf_detect(AsmFormatPlugin * format);
static int _elf_decode(AsmFormatPlugin * format, int raw);
static int _elf_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);


/* public */
/* variables */
/* format_plugin */
AsmFormatPluginDefinition format_plugin =
{
	"elf",
	"ELF",
	LICENSE_GNU_LGPL3_FLAGS,
	ELFMAG,
	SELFMAG,
	_elf_init,
	_elf_destroy,
	_elf_guess,
	NULL,
	_elf_section,
	_elf_detect,
	_elf_decode,
	_elf_decode_section
};


/* private */
/* functions */
/* plug-in */
/* elf_init */
static const ElfArch * _init_arch(char const * arch);

static AsmFormatPlugin * _elf_init(AsmFormatPluginHelper * helper,
		char const * arch)
{
	Elf * elf;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, arch);
#endif
	if((elf = object_new(sizeof(*elf))) == NULL)
		return NULL;
	elf->helper = helper;
	elf->destroy = NULL;
	elf->decode = NULL;
	elf->shstrtab.buf = NULL;
	elf->shstrtab.cnt = 0;
	elf->es32 = NULL;
	elf->es32_cnt = 0;
	elf->es64 = NULL;
	elf->es64_cnt = 0;
	if(arch == NULL)
	{
		elf->arch = NULL;
		return elf;
	}
	if((elf->arch = _init_arch(arch)) == NULL)
	{
		object_delete(elf);
		return NULL;
	}
	if(elf->arch->capacity == ELFCLASS32)
	{
		if(elf32_init(elf) != 0)
			return NULL;
		elf->destroy = elf32_destroy;
		elf->section = elf32_section;
		elf->decode = elf32_decode;
	}
	else if(elf->arch->capacity == ELFCLASS64)
	{
		if(elf64_init(elf) != 0)
			return NULL;
		elf->destroy = elf64_destroy;
		elf->section = elf64_section;
		elf->decode = elf64_decode;
	}
	else
		return NULL;
	return elf;
}

static const ElfArch * _init_arch(char const * arch)
{
	const ElfArch * ea;

	for(ea = elf_arch; ea->arch != NULL; ea++)
		if(strcmp(ea->arch, arch) == 0)
			return ea;
	error_set_code(1, "%s: %s", arch, "Unsupported ELF architecture");
	return NULL;
}


/* elf_destroy */
static int _elf_destroy(AsmFormatPlugin * format)
{
	Elf * elf = format;
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(format->destroy != NULL)
		ret = format->destroy(elf);
	free(elf->es32);
	free(elf->es64);
	object_delete(elf);
	return ret;
}


/* elf_section */
static int _elf_section(AsmFormatPlugin * format, char const * name)
{
	if(format->section == NULL)
		return -1;
	return format->section(format, name);
}


/* elf_guess */
static char const * _elf_guess(AsmFormatPlugin * format, char const * hint)
{
	/* XXX share these tables with _elf_detect() */
	struct
	{
		char const * quirk;
		char const * arch;
	} quirks[] =
	{
		{ "arm", "armel" },
		{ "mips", "mipsel" },
		{ "x86", "i686" },
		{ "x86-64", "amd64" },
		{ "x86_64", "amd64" }
	};
	char const * arch[] =
	{
		"alpha",
		"amd64",
		"armeb", "armel",
		"i386", "i486", "i586", "i686",
		"mips", "mips64",
		"sparc", "sparc64",
	};
	size_t i;
	(void) format;

	if(hint == NULL)
		return NULL;
	for(i = 0; i < sizeof(quirks) / sizeof(*quirks); i++)
		if(string_compare(hint, quirks[i].quirk) == 0)
			return quirks[i].arch;
	for(i = 0; i < sizeof(arch) / sizeof(*arch); i++)
		if(string_compare(hint, arch[i]) == 0)
			return hint;
	return NULL;
}


/* elf_detect */
static char const * _elf_detect(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	char const * ret;
	union
	{
		Elf32_Ehdr ehdr32;
		Elf64_Ehdr ehdr64;
	} ehdr;

	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return NULL;
	if(helper->read(helper->format, &ehdr, sizeof(ehdr)) != sizeof(ehdr))
		return NULL;
	switch(ehdr.ehdr32.e_ident[EI_CLASS])
	{
		case ELFCLASS32:
			if((ret = elf32_detect(format, &ehdr.ehdr32)) != NULL)
				format->decode = elf32_decode;
			break;
		case ELFCLASS64:
			if((ret = elf64_detect(format, &ehdr.ehdr64)) != NULL)
				format->decode = elf64_decode;
			break;
		default:
			ret = NULL;
			error_set_code(1, "%s: %s 0x%x\n",
					helper->get_filename(helper->format),
					"Unsupported ELF class",
					ehdr.ehdr32.e_ident[EI_CLASS]);
			break;
	}
	return ret;
}


/* elf_decode */
static int _elf_decode(AsmFormatPlugin * format, int raw)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%d)\n", __func__, raw);
#endif
	if(_elf_detect(format) == NULL)
		return -1;
	return format->decode(format, raw);
}


/* elf_decode_section */
static int _elf_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	return helper->decode(helper->format, section->offset, section->size,
			section->base, calls, calls_cnt);
}
