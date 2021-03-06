/* $Id$ */
/* Copyright (c) 2015-2018 Pierre Pronchery <khorben@defora.org> */
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



#include <System.h>
#define ELFSIZE 64
#include "elf.h"

#include "elf.c"


/* ELF64 */
static void _swap_64_ehdr(Elf64_Ehdr * ehdr);
static void _swap_64_phdr(Elf64_Phdr * phdr);
static void _swap_64_shdr(Elf64_Shdr * shdr);


/* ELF64 */
/* elf64_detect */
char const * elf64_detect(AsmFormatPlugin * format, Elf64_Ehdr * ehdr)
{
	(void) format;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(ehdr->e_ident[EI_DATA] != elf_arch_native->endian)
		_swap_64_ehdr(ehdr);
	switch(ehdr->e_machine)
	{
#if defined(EM_AMD64)
		case EM_AMD64:
			return "amd64";
#elif defined(EM_X86_64)
		case EM_X86_64:
			return "amd64";
#endif
		case EM_SPARC:
		case EM_SPARCV9:
			return "sparc64";
	}
	error_set_code(1, "%s: %s 0x%x", "elf", "Unsupported ELF architecture",
			ehdr->e_machine);
	return NULL;
}


/* destroy_64 */
static int _destroy_64_phdr(AsmFormatPlugin * format, Elf64_Off offset);
static int _destroy_64_shdr(AsmFormatPlugin * format, Elf64_Off offset);

int elf64_destroy(AsmFormatPlugin * format)
{
	int ret = 0;
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format;
	long offset;

	if(elf64_section(format, ".shstrtab") != 0)
		ret = 1;
	else if(helper->write(helper->format, elf->shstrtab.buf,
				elf->shstrtab.cnt)
			!= (ssize_t)elf->shstrtab.cnt)
		ret = -1;
	else if((offset = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
		ret = -1;
	else if(_destroy_64_phdr(format, offset) != 0
			|| _destroy_64_shdr(format, offset) != 0)
		ret = 1;
	free(elf->shstrtab.buf);
	elf->shstrtab.buf = NULL;
	elf->shstrtab.cnt = 0;
	return ret;
}

static int _destroy_64_phdr(AsmFormatPlugin * format, Elf64_Off offset)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format;
	const ElfArch * ea = elf->arch;
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

static int _destroy_64_shdr(AsmFormatPlugin * format, Elf64_Off offset)
{
	AsmFormatPluginHelper * helper = format->helper;
	Elf * elf = format;
	const ElfArch * ea = elf->arch;
	Elf64_Xword addralign = ea->addralign;
	Elf64_Shdr * es64 = elf->es64;
	Elf64_Shdr hdr;
	int i;

	if(helper->seek(helper->format, 0, SEEK_END) < 0)
		return elf_error(format);
	/* write the undefined section reference */
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
	/* write the sections */
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


/* decode_64 */
static int _decode64_shdr(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Shdr ** shdr);
static int _decode64_addr(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Addr * addr);
static int _decode64_strtab(AsmFormatPlugin * format, Elf64_Shdr * shdr,
		size_t shdr_cnt, uint16_t ndx, char ** strtab,
		size_t * strtab_cnt);
static int _decode64_symtab(AsmFormatPlugin * format, Elf64_Ehdr * ehdr,
		Elf64_Shdr * shdr, size_t shdr_cnt, uint16_t ndx);

int elf64_decode(AsmFormatPlugin * format, int raw)
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
	if(_decode64_addr(format, &ehdr, &base) != 0)
	{
		free(shdr);
		return -1;
	}
	/* XXX ignore errors */
	_decode64_strtab(format, shdr, ehdr.e_shnum, ehdr.e_shstrndx,
			&shstrtab, &shstrtab_cnt);
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
				&& helper->set_section(helper->format, i, 0,
					&shstrtab[shdr[i].sh_name],
					shdr[i].sh_offset, shdr[i].sh_size,
					base + shdr[i].sh_offset) == NULL)
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

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %u\n", __func__, ehdr->e_shentsize);
#endif
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
		return -elf_error(format);
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

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
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

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u) %lu\n", __func__, ndx, shdr_cnt);
#endif
	if(ndx >= shdr_cnt)
		return -error_set_code(1, "%s: %s",
				helper->get_filename(helper->format),
				"Unable to read the string table");
	shdr = &shdr[ndx];
	if(shdr->sh_size == 0)
	{
		*strtab = NULL;
		*strtab_cnt = 0;
		return 0;
	}
	if(helper->seek(helper->format, shdr->sh_offset, SEEK_SET) < 0)
		return -1;
	size = sizeof(**strtab) * shdr->sh_size;
	if((*strtab = malloc(size)) == NULL)
		return -elf_error(format);
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
