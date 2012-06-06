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
/* FIXME:
 * - ensure the first section output is of type SHN_UNDEF
 * - use set_string() to store and remember strings? */



#include <System.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include "Asm.h"

/* portability */
#define Elf64_Quarter		unsigned char
#ifndef EM_486
# define EM_486			6
#endif


/* ELF */
/* private */
/* types */
typedef struct _ElfArch
{
	char const * arch;
	unsigned char machine;
	unsigned char capacity;
	unsigned char endian;
	uint64_t addralign;
} ElfArch;

typedef struct _ElfSectionValues
{
	char const * name;
	Elf32_Word type;	/* works for 64-bit too */
	Elf32_Word flags;
} ElfSectionValues;

typedef struct _ElfStrtab
{
	char * buf;
	size_t cnt;
} ElfStrtab;

typedef struct _Elf
{
	ElfArch * arch;

	/* ELF32 */
	Elf32_Shdr * es32;
	int es32_cnt;

	/* ELF64 */
	Elf64_Shdr * es64;
	int es64_cnt;
} Elf;


/* prototypes */
static int _elf_error(AsmFormatPlugin * format);

/* plug-in */
static int _elf_init(AsmFormatPlugin * format, char const * arch);
static int _elf_exit(AsmFormatPlugin * format);
static char const * _elf_detect(AsmFormatPlugin * format);
static int _elf_decode(AsmFormatPlugin * format, int raw);
static int _elf_decode32(AsmFormatPlugin * format, int raw);
static int _elf_decode64(AsmFormatPlugin * format, int raw);
static int _elf_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);

/* ELF32 */
static int _init_32(AsmFormatPlugin * format);
static int _exit_32(AsmFormatPlugin * format);
static int _section_32(AsmFormatPlugin * format, char const * name);
static void _swap_32_ehdr(Elf32_Ehdr * ehdr);
static void _swap_32_phdr(Elf32_Phdr * phdr);
static void _swap_32_shdr(Elf32_Shdr * shdr);

/* ELF64 */
static int _init_64(AsmFormatPlugin * format);
static int _exit_64(AsmFormatPlugin * format);
static int _section_64(AsmFormatPlugin * format, char const * name);
static void _swap_64_ehdr(Elf64_Ehdr * ehdr);
static void _swap_64_phdr(Elf64_Phdr * phdr);
static void _swap_64_shdr(Elf64_Shdr * shdr);

/* ElfStrtab */
static int _elfstrtab_set(AsmFormatPlugin * format, ElfStrtab * strtab,
		char const * name);


/* variables */
static ElfArch elf_arch[] =
{
	{ "amd64",	EM_X86_64,	ELFCLASS64,	ELFDATA2LSB, 0x4 },
	{ "arm",	EM_ARM,		ELFCLASS32,	ELFDATA2LSB, 0x0 },
	{ "armeb",	EM_ARM,		ELFCLASS32,	ELFDATA2MSB, 0x0 },
	{ "armel",	EM_ARM,		ELFCLASS32,	ELFDATA2LSB, 0x0 },
	{ "i386",	EM_386,		ELFCLASS32,	ELFDATA2LSB, 0x4 },
	{ "i486",	EM_386,		ELFCLASS32,	ELFDATA2LSB, 0x4 },
	{ "i586",	EM_386,		ELFCLASS32,	ELFDATA2LSB, 0x4 },
	{ "i686",	EM_386,		ELFCLASS32,	ELFDATA2LSB, 0x4 },
	{ "mips",	EM_MIPS,	ELFCLASS32,	ELFDATA2MSB, 0x0 },
	{ "mipseb",	EM_MIPS,	ELFCLASS32,	ELFDATA2MSB, 0x0 },
	{ "mipsel",	EM_MIPS,	ELFCLASS32,	ELFDATA2LSB, 0x0 },
	{ "sparc",	EM_SPARC,	ELFCLASS32,	ELFDATA2MSB, 0x0 },
	{ "sparc64",	EM_SPARCV9,	ELFCLASS64,	ELFDATA2MSB, 0x0 },
	{ NULL,		'\0',		'\0',		'\0',        0x0 }
};
#if defined(__amd64__)
static const ElfArch * elf_arch_native = &elf_arch[0];
#elif defined(__arm__)
static const ElfArch * elf_arch_native = &elf_arch[1];
#elif defined(__i386__)
static const ElfArch * elf_arch_native = &elf_arch[2];
#elif defined(__sparc64__)
static const ElfArch * elf_arch_native = &elf_arch[8];
#elif defined(__sparc__)
static const ElfArch * elf_arch_native = &elf_arch[7];
#else
# error "Unsupported architecture"
#endif

static ElfSectionValues elf_section_values[] =
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

static ElfStrtab shstrtab = { NULL, 0 };	/* section string table */


/* public */
/* variables */
/* format_plugin */
AsmFormatPlugin format_plugin =
{
	NULL,
	"elf",
	ELFMAG,
	SELFMAG,
	_elf_init,
	_elf_exit,
	NULL,
	NULL,
	_elf_detect,
	_elf_decode,
	_elf_decode_section,
	NULL
};


/* private */
/* functions */
/* elf_error */
static int _elf_error(AsmFormatPlugin * format)
{
	return -error_set_code(1, "%s: %s", format->helper->get_filename(
				format->helper->format), strerror(errno));
}


/* elf_init */
static ElfArch * _init_arch(char const * arch);

static int _elf_init(AsmFormatPlugin * format, char const * arch)
{
	Elf * elf;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, arch);
#endif
	if((elf = object_new(sizeof(*elf))) == NULL)
		return -1;
	format->priv = elf;
	elf->es32 = NULL;
	elf->es32_cnt = 0;
	elf->es64 = NULL;
	elf->es64_cnt = 0;
	if(arch == NULL)
	{
		elf->arch = NULL;
		return 0;
	}
	if((elf->arch = _init_arch(arch)) == NULL)
	{
		object_delete(elf);
		return -1;
	}
	if(elf->arch->capacity == ELFCLASS32)
	{
		if(_init_32(format) != 0)
			return -1;
		format->exit = _exit_32;
		format->section = _section_32;
	}
	else if(elf->arch->capacity == ELFCLASS64)
	{
		if(_init_64(format) != 0)
			return -1;
		format->exit = _exit_64;
		format->section = _section_64;
	}
	else
		return -1;
	return 0;
}

static ElfArch * _init_arch(char const * arch)
{
	ElfArch * ea;

	for(ea = elf_arch; ea->arch != NULL; ea++)
		if(strcmp(ea->arch, arch) == 0)
			return ea;
	error_set_code(1, "%s: %s", arch, "Unsupported ELF architecture");
	return NULL;
}


/* elf_exit */
static int _elf_exit(AsmFormatPlugin * format)
{
	Elf * elf = format->priv;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	free(elf->es32);
	free(elf->es64);
	object_delete(elf);
	return 0;
}


/* elf_detect */
static char const * _detect_32(AsmFormatPlugin * format, Elf32_Ehdr * ehdr);
static char const * _detect_64(AsmFormatPlugin * format, Elf64_Ehdr * ehdr);

static char const * _elf_detect(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
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
			return _detect_32(format, &ehdr.ehdr32);
		case ELFCLASS64:
			return _detect_64(format, &ehdr.ehdr64);
	}
	error_set_code(1, "%s: %s 0x%x\n", helper->get_filename(helper->format),
			"Unsupported ELF class", ehdr.ehdr32.e_ident[EI_CLASS]);
	return NULL;
}

static char const * _detect_32(AsmFormatPlugin * format, Elf32_Ehdr * ehdr)
{
	format->decode = _elf_decode32;
	if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
		_swap_32_ehdr(ehdr);
	switch(ehdr->e_machine)
	{
		case EM_386:
		case EM_486:
			return "i686";
		case EM_ALPHA:
			return "alpha";
		case EM_ARM:
			return "arm";
		case EM_MIPS:
			return "mips";
		case EM_SPARC:
			return "sparc";
	}
	format->decode = _elf_decode;
	error_set_code(1, "%s: %s 0x%x", "elf", "Unsupported ELF architecture",
			ehdr->e_machine);
	return NULL;
}

static char const * _detect_64(AsmFormatPlugin * format, Elf64_Ehdr * ehdr)
{
	format->decode = _elf_decode64;
	if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
		_swap_64_ehdr(ehdr);
	switch(ehdr->e_machine)
	{
		case EM_SPARC:
		case EM_SPARCV9:
			return "sparc64";
		case EM_X86_64:
			return "amd64";
	}
	format->decode = _elf_decode;
	error_set_code(1, "%s: %s 0x%x", "elf", "Unsupported ELF architecture",
			ehdr->e_machine);
	return NULL;
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


/* elf_decode32 */
static int _decode32_shdr(AsmFormatPlugin * format, Elf32_Ehdr * ehdr,
		Elf32_Shdr ** shdr);
static int _decode32_addr(AsmFormatPlugin * format, Elf32_Ehdr * ehdr,
		Elf32_Addr * addr);
static int _decode32_strtab(AsmFormatPlugin * format, Elf32_Shdr * shdr,
		size_t shdr_cnt, uint16_t ndx, char ** strtab,
		size_t * strtab_cnt);
static int _decode32_symtab(AsmFormatPlugin * format, Elf32_Ehdr * ehdr,
		Elf32_Shdr * shdr, size_t shdr_cnt, uint16_t ndx);

static int _elf_decode32(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf32_Ehdr ehdr;
	Elf32_Shdr * shdr = NULL;
	Elf32_Addr base = 0x0;
	char * shstrtab = NULL;
	size_t shstrtab_cnt = 0;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() \"%s\"\n", __func__,
			helper->get_filename(helper->format));
#endif
	if(helper->seek(helper->format, 0, SEEK_SET) != 0
			|| helper->read(helper->format, &ehdr, sizeof(ehdr))
			!= sizeof(ehdr))
		return -1;
	if(ehdr.e_ident[EI_DATA] != elf_arch_native->endian)
		_swap_32_ehdr(&ehdr);
	if(_decode32_shdr(format, &ehdr, &shdr) != 0)
		return -1;
	if(_decode32_addr(format, &ehdr, &base) != 0
			|| _decode32_strtab(format, shdr, ehdr.e_shnum,
				ehdr.e_shstrndx, &shstrtab, &shstrtab_cnt)
			!= 0)
	{
		free(shdr);
		return -1;
	}
	for(i = 0; i < ehdr.e_shnum; i++)
		if(shdr[i].sh_type == SHT_SYMTAB)
		{
			/* XXX ignore errors? */
			_decode32_symtab(format, &ehdr, shdr, ehdr.e_shnum, i);
			break;
		}
	for(i = 0; i < ehdr.e_shnum; i++)
	{
		if(shdr[i].sh_name >= shstrtab_cnt)
			continue;
		if((raw || (shdr[i].sh_type == SHT_PROGBITS && shdr[i].sh_flags
						& SHF_EXECINSTR))
				&& helper->set_section(helper->format, i,
					&shstrtab[shdr[i].sh_name],
					shdr[i].sh_offset, shdr[i].sh_size,
					base + shdr[i].sh_offset) < 0)
			break;
	}
	free(shstrtab);
	free(shdr);
	return (i == ehdr.e_shnum) ? 0 : -1;
}

static int _decode32_shdr(AsmFormatPlugin * format, Elf32_Ehdr * ehdr,
		Elf32_Shdr ** shdr)
{
	AsmFormatPluginHelper * helper = format->helper;
	ssize_t size;
	size_t i;

	if(ehdr->e_shentsize == 0)
	{
		*shdr = NULL;
		return 0;
	}
	if(ehdr->e_shentsize != sizeof(**shdr))
		return -error_set_code(1, "%s: %s",
				helper->get_filename(helper->format),
				"Invalid section header size");
	if(helper->seek(helper->format, ehdr->e_shoff, SEEK_SET) < 0)
		return -1;
	size = sizeof(**shdr) * ehdr->e_shnum;
	if((*shdr = malloc(size)) == NULL)
		return -_elf_error(format);
	if(helper->read(helper->format, *shdr, size) != size)
	{
		free(*shdr);
		return -1;
	}
	if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
		for(i = 0; i < ehdr->e_shnum; i++)
			_swap_32_shdr(*shdr + i);
	return 0;
}

static int _decode32_addr(AsmFormatPlugin * format, Elf32_Ehdr * ehdr,
		Elf32_Addr * addr)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf32_Half i;
	Elf32_Phdr phdr;

	if(helper->seek(helper->format, ehdr->e_phoff, SEEK_SET) < 0)
		return -1;
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		if(helper->read(helper->format, &phdr, sizeof(phdr))
				!= sizeof(phdr))
			return -1;
		if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
			_swap_32_phdr(&phdr);
		if(phdr.p_type == PT_LOAD && phdr.p_flags & (PF_R | PF_X))
		{
			*addr = phdr.p_vaddr;
			return 0;
		}
	}
	*addr = 0x0;
	return 0;
}

static int _decode32_strtab(AsmFormatPlugin * format, Elf32_Shdr * shdr,
		size_t shdr_cnt, uint16_t ndx, char ** strtab,
		size_t * strtab_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;

	if(ndx >= shdr_cnt)
		return -error_set_code(1, "%s: %s",
				helper->get_filename(helper->format),
				"Unable to read the string table");
	shdr = &shdr[ndx];
	if(helper->seek(helper->format, shdr->sh_offset, SEEK_SET) < 0)
		return -1;
	if((*strtab = malloc(shdr->sh_size)) == NULL)
		return -_elf_error(format);
	if(helper->read(helper->format, *strtab, shdr->sh_size)
			!= shdr->sh_size)
	{
		free(*strtab);
		return -1;
	}
	*strtab_cnt = shdr->sh_size;
	return 0;
}

static int _decode32_symtab(AsmFormatPlugin * format, Elf32_Ehdr * ehdr,
		Elf32_Shdr * shdr, size_t shdr_cnt, uint16_t ndx)
{
	AsmFormatPluginHelper * helper = format->helper;
	char * strtab = NULL;
	size_t strtab_cnt = 0;
	Elf32_Sym sym;
	size_t i;
	off_t offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(ndx >= shdr_cnt || shdr[ndx].sh_entsize != sizeof(sym))
		return -1;
	if(_decode32_strtab(format, shdr, shdr_cnt, shdr[ndx].sh_link, &strtab,
				&strtab_cnt) != 0)
		return -1;
	/* read and process symbols */
	if((offset = helper->seek(helper->format, shdr[ndx].sh_offset,
					SEEK_SET)) < 0
			|| (unsigned long)offset != shdr[ndx].sh_offset)
	{
		free(strtab);
		return -1;
	}
	for(i = 0; i * sizeof(sym) < shdr[ndx].sh_size; i++)
		if(helper->read(helper->format, &sym, sizeof(sym))
				!= sizeof(sym))
			break;
		else if(sym.st_name >= strtab_cnt)
			break;
		else if(ELF32_ST_TYPE(sym.st_info) == STT_FUNC)
		{
			offset = -1;
			if(ehdr->e_type == ET_REL || ehdr->e_type == ET_EXEC
					|| ehdr->e_type == ET_DYN)
				offset = sym.st_value;
			/* record the function */
			helper->set_function(helper->format, i,
					&strtab[sym.st_name], sym.st_value,
					sym.st_size);
		}
	if(i * sizeof(sym) != shdr[ndx].sh_size)
	{
		free(strtab);
		return -1;
	}
	return 0;
}


/* elf_decode64 */
static int _decode64_shdr(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Shdr ** shdr);
static int _decode64_addr(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Addr * addr);
static int _decode64_strtab(AsmFormatPlugin * format, Elf64_Shdr * shdr,
		size_t shdr_cnt, uint16_t ndx, char ** strtab,
		size_t * strtab_cnt);
static int _decode64_symtab(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Shdr * shdr, size_t shdr_cnt, uint16_t ndx);

static int _elf_decode64(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf64_Ehdr ehdr;
	Elf64_Shdr * shdr = NULL;
	Elf64_Addr base = 0x0;
	char * shstrtab = NULL;
	size_t shstrtab_cnt = 0;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() \"%s\"\n", __func__,
			helper->get_filename(helper->format));
#endif
	if(helper->seek(helper->format, 0, SEEK_SET) != 0
			|| helper->read(helper->format, &ehdr, sizeof(ehdr))
			!= sizeof(ehdr))
		return -1;
	if(ehdr.e_ident[EI_DATA] != elf_arch_native->endian)
		_swap_64_ehdr(&ehdr);
	if(_decode64_shdr(format, &ehdr, &shdr) != 0)
		return -1;
	if(_decode64_addr(format, &ehdr, &base) != 0
			|| _decode64_strtab(format, shdr, ehdr.e_shnum,
				ehdr.e_shstrndx, &shstrtab, &shstrtab_cnt) != 0)
	{
		free(shdr);
		return -1;
	}
	for(i = 0; i < ehdr.e_shnum; i++)
		if(shdr[i].sh_type == SHT_SYMTAB)
		{
			/* XXX ignore errors? */
			_decode64_symtab(format, &ehdr, shdr, ehdr.e_shnum, i);
			break;
		}
	for(i = 0; i < ehdr.e_shnum; i++)
	{
		if(shdr[i].sh_name >= shstrtab_cnt)
			continue;
		if((raw || (shdr[i].sh_type == SHT_PROGBITS && shdr[i].sh_flags
						& SHF_EXECINSTR))
				&& helper->set_section(helper->format, i,
					&shstrtab[shdr[i].sh_name],
					shdr[i].sh_offset, shdr[i].sh_size,
					base + shdr[i].sh_offset) < 0)
			break;
	}
	free(shstrtab);
	free(shdr);
	return (i == ehdr.e_shnum) ? 0 : -1;
}

static int _decode64_shdr(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Shdr ** shdr)
{
	AsmFormatPluginHelper * helper = format->helper;
	ssize_t size;
	size_t i;

	if(ehdr->e_shentsize == 0)
	{
		*shdr = NULL;
		return 0;
	}
	if(ehdr->e_shentsize != sizeof(**shdr))
		return -error_set_code(1, "%s: %s",
				helper->get_filename(helper->format),
				"Invalid section header size");
	if(helper->seek(helper->format, ehdr->e_shoff, SEEK_SET) < 0)
		return -1;
	size = sizeof(**shdr) * ehdr->e_shnum;
	if((*shdr = malloc(size)) == NULL)
		return -_elf_error(format);
	if(helper->read(helper->format, *shdr, size) != size)
	{
		free(*shdr);
		return -1;
	}
	if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
		for(i = 0; i < ehdr->e_shnum; i++)
			_swap_64_shdr(*shdr + i);
	return 0;
}

static int _decode64_addr(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Addr * addr)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf64_Quarter i;
	Elf64_Phdr phdr;

	if(helper->seek(helper->format, ehdr->e_phoff, SEEK_SET) < 0)
		return -1;
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		if(helper->read(helper->format, &phdr, sizeof(phdr))
				!= sizeof(phdr))
			return -1;
		if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
			_swap_64_phdr(&phdr);
		if(phdr.p_type == PT_LOAD && phdr.p_flags & (PF_R | PF_X))
		{
			*addr = phdr.p_vaddr;
			return 0;
		}
	}
	*addr = 0x0;
	return 0;
}

static int _decode64_strtab(AsmFormatPlugin * format, Elf64_Shdr * shdr,
		size_t shdr_cnt, uint16_t ndx, char ** strtab,
		size_t * strtab_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	ssize_t size;

	if(ndx >= shdr_cnt)
		return -error_set_code(1, "%s: %s",
				helper->get_filename(helper->format),
				"Unable to read the string table");
	shdr = &shdr[ndx];
	if(helper->seek(helper->format, shdr->sh_offset, SEEK_SET) < 0)
		return -1;
	size = sizeof(**strtab) * shdr->sh_size;
	if((*strtab = malloc(size)) == NULL)
		return -_elf_error(format);
	if(helper->read(helper->format, *strtab, size) != size)
	{
		free(*strtab);
		return -1;
	}
	*strtab_cnt = shdr->sh_size;
	return 0;
}

static int _decode64_symtab(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Shdr * shdr, size_t shdr_cnt, uint16_t ndx)
{
	AsmFormatPluginHelper * helper = format->helper;
	char * strtab = NULL;
	size_t strtab_cnt = 0;
	Elf64_Sym sym;
	size_t i;
	off_t offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(ndx >= shdr_cnt || shdr[ndx].sh_entsize != sizeof(sym))
		return -1;
	if(_decode64_strtab(format, shdr, shdr_cnt, shdr[ndx].sh_link, &strtab,
				&strtab_cnt) != 0)
		return -1;
	/* read and process symbols */
	if((offset = helper->seek(helper->format, shdr[ndx].sh_offset,
					SEEK_SET)) < 0
			|| (unsigned long)offset != shdr[ndx].sh_offset)
	{
		free(strtab);
		return -1;
	}
	for(i = 0; i * sizeof(sym) < shdr[ndx].sh_size; i++)
		if(helper->read(helper->format, &sym, sizeof(sym))
				!= sizeof(sym))
			break;
		else if(sym.st_name >= strtab_cnt)
			break;
		else if(ELF64_ST_TYPE(sym.st_info) == STT_FUNC)
		{
			offset = -1;
			if(ehdr->e_type == ET_REL || ehdr->e_type == ET_EXEC
					|| ehdr->e_type == ET_DYN)
				offset = sym.st_value;
			/* record the function */
			helper->set_function(helper->format, i,
					&strtab[sym.st_name], offset,
					sym.st_size);
		}
	if(i * sizeof(sym) != shdr[ndx].sh_size)
	{
		free(strtab);
		return -1;
	}
	return 0;
}


/* elf_decode_section */
static int _elf_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;

	return helper->decode(helper->format, section->offset, section->size,
			section->base, calls, calls_cnt);
}


/* section_values */
static ElfSectionValues * _section_values(char const * name)
{
	ElfSectionValues * esv;
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


/* ELF32 */
/* init_32 */
static int _init_32(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	ElfArch * ea = elf->arch;
	Elf32_Ehdr hdr;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	memset(&hdr, 0, sizeof(hdr));
	memcpy(&hdr.e_ident, ELFMAG, SELFMAG);
	hdr.e_ident[EI_CLASS] = ELFCLASS32;
	hdr.e_ident[EI_DATA] = ea->endian;
	hdr.e_ident[EI_VERSION] = EV_CURRENT;
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.e_type = _htob16(ET_REL);
		hdr.e_machine = _htob16(ea->machine);
		hdr.e_version = _htob32(EV_CURRENT);
		hdr.e_ehsize = _htob16(sizeof(hdr));
		hdr.e_shentsize = _htob16(sizeof(Elf32_Shdr));
		hdr.e_shstrndx = _htob16(SHN_UNDEF);
	}
	else
	{
		hdr.e_type = _htol16(ET_REL);
		hdr.e_machine = _htol16(ea->machine);
		hdr.e_version = _htol32(EV_CURRENT);
		hdr.e_ehsize = _htol16(sizeof(hdr));
		hdr.e_shentsize = _htol16(sizeof(Elf32_Shdr));
		hdr.e_shstrndx = _htol16(SHN_UNDEF);
	}
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return _elf_error(format);
	return 0;
}


/* exit_32 */
static int _exit_32_phdr(AsmFormatPlugin * format, Elf32_Off offset);
static int _exit_32_shdr(AsmFormatPlugin * format, Elf32_Off offset);

static int _exit_32(AsmFormatPlugin * format)
{
	int ret = 0;
	AsmFormatPluginHelper * helper = format->helper;
	long offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(_section_32(format, ".shstrtab") != 0)
		ret = -1;
	else if(helper->write(helper->format, shstrtab.buf, shstrtab.cnt)
			!= (ssize_t)shstrtab.cnt)
		ret = -1;
	else if((offset = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
		ret = -1;
	else if(_exit_32_phdr(format, offset) != 0
			|| _exit_32_shdr(format, offset) != 0)
		ret = -1;
	free(shstrtab.buf);
	shstrtab.buf = NULL;
	shstrtab.cnt = 0;
	ret |= _elf_exit(format);
	return ret;
}

static int _exit_32_phdr(AsmFormatPlugin * format, Elf32_Off offset)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	ElfArch * ea = elf->arch;
	Elf32_Ehdr hdr;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(elf->es32_cnt == 0)
		return 0;
	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return -1;
	if(helper->read(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return _elf_error(format);
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.e_shoff = _htob32(offset);
		hdr.e_shnum = _htob16(elf->es32_cnt + 1);
		hdr.e_shstrndx = _htob16(elf->es32_cnt);
	}
	else
	{
		hdr.e_shoff = _htol32(offset);
		hdr.e_shnum = _htol16(elf->es32_cnt + 1);
		hdr.e_shstrndx = _htol16(elf->es32_cnt);
	}
	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return -1;
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	return 0;
}

static int _exit_32_shdr(AsmFormatPlugin * format, Elf32_Off offset)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	ElfArch * ea = elf->arch;
	Elf32_Word addralign = ea->addralign;
	Elf32_Shdr * es32 = elf->es32;
	Elf32_Shdr hdr;
	int i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(helper->seek(helper->format, 0, SEEK_END) < 0)
		return _elf_error(format);
	memset(&hdr, 0, sizeof(hdr));
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.sh_type = _htob32(SHT_NULL);
		hdr.sh_link = _htob32(SHN_UNDEF);
	}
	else
	{
		hdr.sh_type = _htol32(SHT_NULL);
		hdr.sh_link = _htol32(SHN_UNDEF);
	}
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	for(i = 0; i < elf->es32_cnt; i++)
	{
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() %d\n", __func__, i);
#endif
		if(i + 1 == elf->es32_cnt)
			es32[i].sh_size = offset - es32[i].sh_offset;
		else
			es32[i].sh_size = es32[i + 1].sh_offset
				- es32[i].sh_offset;
		es32[i].sh_offset = (ea->endian == ELFDATA2MSB)
			? _htob32(es32[i].sh_offset)
			: _htol32(es32[i].sh_offset);
		es32[i].sh_size = (ea->endian == ELFDATA2MSB)
			? _htob32(es32[i].sh_size) : _htol32(es32[i].sh_size);
		if(es32[i].sh_type == SHT_PROGBITS)
			es32[i].sh_addralign = (ea->endian == ELFDATA2MSB)
				? _htob32(addralign) : _htol32(addralign);
		es32[i].sh_type = (ea->endian == ELFDATA2MSB)
			? _htob32(es32[i].sh_type) : _htol32(es32[i].sh_type);
		if(helper->write(helper->format, &es32[i], sizeof(Elf32_Shdr))
				!= sizeof(Elf32_Shdr))
			return -1;
	}
	return 0;
}


/* section_32 */
static int _section_32(AsmFormatPlugin * format, char const * name)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	int ss;
	Elf32_Shdr * p;
	ElfSectionValues * esv;
	long offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	if((ss = _elfstrtab_set(format, &shstrtab, name)) < 0)
		return -1;
	if((p = realloc(elf->es32, sizeof(*p) * (elf->es32_cnt + 1))) == NULL)
		return _elf_error(format);
	elf->es32 = p;
	p = &elf->es32[elf->es32_cnt++];
	memset(p, 0, sizeof(*p));
	esv = _section_values(name);
	p->sh_name = ss;
	p->sh_type = esv->type;
	p->sh_flags = esv->flags;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %s\n", __func__, "before seek()");
#endif
	if((offset = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %s\n", __func__, "after seek()");
#endif
	p->sh_offset = offset;
	p->sh_link = SHN_UNDEF; /* FIXME */
	return 0;
}


/* swap_32_ehdr */
static void _swap_32_ehdr(Elf32_Ehdr * ehdr)
{
	ehdr->e_type = _bswap16(ehdr->e_type);
	ehdr->e_machine = _bswap16(ehdr->e_machine);
	ehdr->e_version = _bswap32(ehdr->e_version);
	ehdr->e_entry = _bswap32(ehdr->e_entry);
	ehdr->e_phoff = _bswap32(ehdr->e_phoff);
	ehdr->e_shoff = _bswap32(ehdr->e_shoff);
	ehdr->e_flags = _bswap32(ehdr->e_flags);
	ehdr->e_ehsize = _bswap16(ehdr->e_ehsize);
	ehdr->e_phentsize = _bswap16(ehdr->e_phentsize);
	ehdr->e_phnum = _bswap16(ehdr->e_phnum);
	ehdr->e_shentsize = _bswap16(ehdr->e_shentsize);
	ehdr->e_shnum = _bswap16(ehdr->e_shnum);
	ehdr->e_shstrndx = _bswap16(ehdr->e_shstrndx);
}


/* swap_32_phdr */
static void _swap_32_phdr(Elf32_Phdr * phdr)
{
	phdr->p_type = _bswap32(phdr->p_type);
	phdr->p_offset = _bswap32(phdr->p_offset);
	phdr->p_vaddr = _bswap32(phdr->p_vaddr);
	phdr->p_paddr = _bswap32(phdr->p_paddr);
	phdr->p_filesz = _bswap32(phdr->p_filesz);
	phdr->p_memsz = _bswap32(phdr->p_memsz);
	phdr->p_flags = _bswap32(phdr->p_flags);
	phdr->p_align = _bswap32(phdr->p_align);
}


/* swap_32_shdr */
static void _swap_32_shdr(Elf32_Shdr * shdr)
{
	shdr->sh_name = _bswap32(shdr->sh_name);
	shdr->sh_type = _bswap32(shdr->sh_type);
	shdr->sh_flags = _bswap32(shdr->sh_flags);
	shdr->sh_addr = _bswap32(shdr->sh_addr);
	shdr->sh_offset = _bswap32(shdr->sh_offset);
	shdr->sh_size = _bswap32(shdr->sh_size);
	shdr->sh_link = _bswap32(shdr->sh_link);
	shdr->sh_info = _bswap32(shdr->sh_info);
	shdr->sh_addralign = _bswap32(shdr->sh_addralign);
	shdr->sh_entsize = _bswap32(shdr->sh_entsize);
}


/* ELF64 */
/* init_64 */
static int _init_64(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	ElfArch * ea = elf->arch;
	Elf64_Ehdr hdr;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	memset(&hdr, 0, sizeof(hdr));
	memcpy(&hdr.e_ident, ELFMAG, SELFMAG);
	hdr.e_ident[EI_CLASS] = ELFCLASS64;
	hdr.e_ident[EI_DATA] = ea->endian;
	hdr.e_ident[EI_VERSION] = EV_CURRENT;
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.e_type = _htob16(ET_REL);
		hdr.e_machine = _htob16(ea->machine);
		hdr.e_version = _htob32(EV_CURRENT);
		hdr.e_ehsize = _htob16(sizeof(hdr));
		hdr.e_shentsize = _htob16(sizeof(Elf64_Shdr));
	}
	else
	{
		hdr.e_type = _htol16(ET_REL);
		hdr.e_machine = _htol16(ea->machine);
		hdr.e_version = _htol32(EV_CURRENT);
		hdr.e_ehsize = _htol16(sizeof(hdr));
		hdr.e_shentsize = _htol16(sizeof(Elf64_Shdr));
	}
	hdr.e_shstrndx = SHN_UNDEF;
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	return 0;
}


/* exit_64 */
static int _exit_64_phdr(AsmFormatPlugin * format, Elf64_Off offset);
static int _exit_64_shdr(AsmFormatPlugin * format, Elf64_Off offset);

static int _exit_64(AsmFormatPlugin * format)
{
	int ret = 0;
	AsmFormatPluginHelper * helper = format->helper;
	long offset;

	if(_section_64(format, ".shstrtab") != 0)
		ret = 1;
	else if(helper->write(helper->format, shstrtab.buf, shstrtab.cnt)
				!= (ssize_t)shstrtab.cnt)
		ret = -1;
	else if((offset = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
		ret = -1;
	else if(_exit_64_phdr(format, offset) != 0
			|| _exit_64_shdr(format, offset) != 0)
		ret = 1;
	free(shstrtab.buf);
	shstrtab.buf = NULL;
	shstrtab.cnt = 0;
	ret |= _elf_exit(format);
	return ret;
}

static int _exit_64_phdr(AsmFormatPlugin * format, Elf64_Off offset)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	ElfArch * ea = elf->arch;
	Elf64_Ehdr hdr;

	if(elf->es64_cnt == 0)
		return 0;
	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return -1;
	if(helper->read(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.e_shoff = _htob64(offset);
		hdr.e_shnum = _htob16(elf->es64_cnt);
		hdr.e_shstrndx = _htob16(elf->es64_cnt - 1);
	}
	else
	{
		hdr.e_shoff = _htol64(offset);
		hdr.e_shnum = _htol16(elf->es64_cnt);
		hdr.e_shstrndx = _htol16(elf->es64_cnt - 1);
	}
	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return -1;
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	return 0;
}

static int _exit_64_shdr(AsmFormatPlugin * format, Elf64_Off offset)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	ElfArch * ea = elf->arch;
	Elf64_Xword addralign = ea->addralign;
	Elf64_Shdr * es64 = elf->es64;
	Elf64_Shdr hdr;
	int i;

	if(helper->seek(helper->format, 0, SEEK_END) < 0)
		return _elf_error(format);
	memset(&hdr, 0, sizeof(hdr));
	if(ea->endian == ELFDATA2MSB)
	{
		hdr.sh_type = _htob32(SHT_NULL);
		hdr.sh_link = _htob32(SHN_UNDEF);
	}
	else
	{
		hdr.sh_type = _htol32(SHT_NULL);
		hdr.sh_link = _htol32(SHN_UNDEF);
	}
	if(helper->write(helper->format, &hdr, sizeof(hdr)) != sizeof(hdr))
		return -1;
	for(i = 0; i < elf->es64_cnt; i++)
	{
		if(i + 1 == elf->es64_cnt)
			es64[i].sh_size = offset - es64[i].sh_offset;
		else
			es64[i].sh_size = es64[i + 1].sh_offset
				- es64[i].sh_offset;
		es64[i].sh_offset = (ea->endian == ELFDATA2MSB)
			? _htob64(es64[i].sh_offset)
			: _htol64(es64[i].sh_offset);
		es64[i].sh_size = (ea->endian == ELFDATA2MSB)
			? _htob64(es64[i].sh_size) : _htol64(es64[i].sh_size);
		if(es64[i].sh_type == SHT_PROGBITS)
			es64[i].sh_addralign = (ea->endian == ELFDATA2MSB)
				? _htob64(addralign) : _htol64(addralign);
		es64[i].sh_type = (ea->endian == ELFDATA2MSB)
			? _htob32(es64[i].sh_type) : _htol32(es64[i].sh_type);
		if(helper->write(helper->format, &es64[i], sizeof(Elf64_Shdr))
				!= sizeof(Elf64_Shdr))
			return -1;
	}
	return 0;
}


/* section_64 */
static int _section_64(AsmFormatPlugin * format, char const * name)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format->priv;
	int ss;
	Elf64_Shdr * p;
	ElfSectionValues * esv;
	long offset;

	if((ss = _elfstrtab_set(format, &shstrtab, name)) < 0)
		return 1;
	if((p = realloc(elf->es64, sizeof(*p) * (elf->es64_cnt + 1))) == NULL)
		return _elf_error(format);
	elf->es64 = p;
	p = &elf->es64[elf->es64_cnt++];
	memset(p, 0, sizeof(*p));
	esv = _section_values(name);
	p->sh_name = ss;
	p->sh_type = esv->type;
	p->sh_flags = esv->flags;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %s\n", __func__, "before seek()");
#endif
	if((offset = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
		return -1;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %s\n", __func__, "after seek()");
#endif
	p->sh_offset = offset;
	p->sh_link = SHN_UNDEF; /* FIXME */
	return 0;
}


/* swap_64_ehdr */
static void _swap_64_ehdr(Elf64_Ehdr * ehdr)
{
	ehdr->e_type = _bswap16(ehdr->e_type);
	ehdr->e_machine = _bswap16(ehdr->e_machine);
	ehdr->e_version = _bswap32(ehdr->e_version);
	ehdr->e_entry = _bswap64(ehdr->e_entry);
	ehdr->e_phoff = _bswap64(ehdr->e_phoff);
	ehdr->e_shoff = _bswap64(ehdr->e_shoff);
	ehdr->e_flags = _bswap32(ehdr->e_flags);
	ehdr->e_ehsize = _bswap16(ehdr->e_ehsize);
	ehdr->e_phentsize = _bswap16(ehdr->e_phentsize);
	ehdr->e_phnum = _bswap16(ehdr->e_phnum);
	ehdr->e_shentsize = _bswap16(ehdr->e_shentsize);
	ehdr->e_shnum = _bswap16(ehdr->e_shnum);
	ehdr->e_shstrndx = _bswap16(ehdr->e_shstrndx);
}


/* swap_64_phdr */
static void _swap_64_phdr(Elf64_Phdr * phdr)
{
	phdr->p_type = _bswap32(phdr->p_type);
	phdr->p_flags = _bswap32(phdr->p_flags);
	phdr->p_offset = _bswap64(phdr->p_offset);
	phdr->p_vaddr = _bswap64(phdr->p_vaddr);
	phdr->p_paddr = _bswap64(phdr->p_paddr);
	phdr->p_filesz = _bswap64(phdr->p_filesz);
	phdr->p_memsz = _bswap64(phdr->p_memsz);
	phdr->p_align = _bswap64(phdr->p_align);
}


/* swap_64_shdr */
static void _swap_64_shdr(Elf64_Shdr * shdr)
{
	shdr->sh_name = _bswap32(shdr->sh_name);
	shdr->sh_type = _bswap32(shdr->sh_type);
	shdr->sh_flags = _bswap64(shdr->sh_flags);
	shdr->sh_addr = _bswap64(shdr->sh_addr);
	shdr->sh_offset = _bswap64(shdr->sh_offset);
	shdr->sh_size = _bswap64(shdr->sh_size);
	shdr->sh_link = _bswap32(shdr->sh_link);
	shdr->sh_info = _bswap32(shdr->sh_info);
	shdr->sh_addralign = _bswap64(shdr->sh_addralign);
	shdr->sh_entsize = _bswap64(shdr->sh_entsize);
}


/* ElfStrtab */
/* private */
/* functions */
/* elfstrtab_get */
static int _elfstrtab_set(AsmFormatPlugin * format, ElfStrtab * strtab,
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
		return -_elf_error(format);
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
