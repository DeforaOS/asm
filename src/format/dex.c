/* $Id$ */
/* Copyright (c) 2011-2013 Pierre Pronchery <khorben@defora.org> */
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
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Asm.h"


/* DEX */
/* private */
/* types */
typedef struct _AsmFormatPlugin Dex;

#pragma pack(1)
typedef struct _DexHeader
{
	char magic[4];
	char _padding0[4];
	uint32_t checksum;
	unsigned char signature[20];
	uint32_t file_size;		/* 0x20 */
	uint32_t header_size;
	uint32_t endian_tag;
	uint32_t link_size;
	uint32_t link_off;		/* 0x30 */
	uint32_t map_off;
	uint32_t string_ids_size;
	uint32_t string_ids_off;
	uint32_t type_ids_size;		/* 0x40 */
	uint32_t type_ids_off;
	uint32_t proto_ids_size;
	uint32_t proto_ids_off;
	uint32_t fields_ids_size;	/* 0x50 */
	uint32_t fields_ids_off;
	uint32_t method_ids_size;
	uint32_t method_ids_off;
	uint32_t class_defs_size;	/* 0x60 */
	uint32_t class_defs_off;
	uint32_t data_size;
	uint32_t data_off;
} DexHeader;

enum
{
	TYPE_HEADER_ITEM	= 0x0000,
	TYPE_STRING_ID_ITEM	= 0x0001,
	TYPE_METHOD_ID_ITEM	= 0x0005,
	TYPE_CODE_ITEM		= 0x2001,
	TYPE_STRING_DATA_ITEM	= 0x2002
};

typedef struct _DexMapItem
{
	uint16_t type;
	uint16_t _padding;
	uint32_t size;
	uint32_t offset;
} DexMapItem;

typedef struct _DexMapCodeItem
{
	uint16_t registers_size;
	uint16_t ins_size;
	uint16_t outs_size;
	uint16_t tries_size;
	uint32_t debug_info_off;
	uint32_t insns_size;
	char insns[0];
} DexMapCodeItem;

typedef struct _DexMapTryItem
{
	uint32_t start_addr;
	uint16_t insn_count;
	uint16_t handler_off;
} DexMapTryItem;

typedef struct _DexMethodIdItem
{
	uint16_t class_idx;
	uint16_t proto_idx;
	uint32_t name_idx;
} DexMethodIdItem;

typedef struct _DexStringIdItem
{
	uint32_t string_data_off;
} DexStringIdItem;
#pragma pack()

typedef struct _DexString
{
	off_t offset;
	char * string;
} DexString;

struct _AsmFormatPlugin
{
	AsmFormatPluginHelper * helper;

	DexMethodIdItem * dmii;
	size_t dmii_cnt;
};


/* variables */
static char _dex_signature[4] = "dex\n";


/* prototypes */
/* plug-in */
static AsmFormatPlugin * _dex_init(AsmFormatPluginHelper * helper,
		char const * arch);
static void _dex_destroy(AsmFormatPlugin * format);
static char const * _dex_detect(AsmFormatPlugin * format);
static int _dex_decode(AsmFormatPlugin * format, int raw);
static int _dex_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);

static int _dex_decode_sleb128(AsmFormatPlugin * format, int32_t * s32);
static int _dex_decode_uleb128(AsmFormatPlugin * format, uint32_t * u32);


/* public */
/* variables */
AsmFormatPluginDefinition format_plugin =
{
	"dex",
	_dex_signature,
	sizeof(_dex_signature),
	_dex_init,
	_dex_destroy,
	NULL,
	NULL,
	_dex_detect,
	_dex_decode,
	_dex_decode_section
};


/* private */
/* functions */
/* plug-in */
/* dex_init */
static AsmFormatPlugin * _dex_init(AsmFormatPluginHelper * helper,
		char const * arch)
{
	Dex * dex;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, arch);
#endif
	if(arch != NULL && strcmp(arch, "dalvik") != 0)
	{
		error_set_code(1, "%s: %s", arch,
				"Unsupported Dex architecture");
		return NULL;
	}
	if((dex = object_new(sizeof(*dex))) == NULL)
		return NULL;
	dex->helper = helper;
	dex->dmii = NULL;
	dex->dmii_cnt = 0;
	return dex;
}


/* dex_destroy */
static void _dex_destroy(AsmFormatPlugin * format)
{
	Dex * dex = format;

	free(dex->dmii);
	object_delete(dex);
}


/* dex_detect */
static char const * _dex_detect(AsmFormatPlugin * format)
{
	/* XXX some sections might contain native code */
	return "dalvik";
}


/* dex_decode */
static int _decode_map(AsmFormatPlugin * format, DexHeader * dh, int raw);
static int _decode_map_code(AsmFormatPlugin * format, size_t id, off_t offset,
		size_t size);
static int _decode_map_method_id(AsmFormatPlugin * format, off_t offset,
		size_t size);
static int _decode_map_string_id(AsmFormatPlugin * format, off_t offset,
		size_t size);

static int _dex_decode(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;
	DexHeader dh;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%d)\n", __func__, raw);
#endif
	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return -1;
	if(helper->read(helper->format, &dh, sizeof(dh)) != sizeof(dh))
		return -1;
	dh.map_off = _htol32(dh.map_off);
	if(_decode_map(format, &dh, raw) != 0)
		return -1;
	return 0;
}

static int _decode_map(AsmFormatPlugin * format, DexHeader * dh, int raw)
{
	int ret = 0;
	AsmFormatPluginHelper * helper = format->helper;
	uint32_t size;
	uint32_t i;
	off_t offset;
	DexMapItem dmi;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(helper->seek(helper->format, dh->map_off, SEEK_SET) != dh->map_off)
		return -1;
	if(helper->read(helper->format, &size, sizeof(size)) != sizeof(size))
		return -1;
	size = _htol32(size);
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %u items\n", __func__, size);
#endif
	for(i = 0; i < size; i++)
	{
		if(helper->read(helper->format, &dmi, sizeof(dmi))
				!= sizeof(dmi))
			return -1;
		offset = helper->seek(helper->format, 0, SEEK_CUR);
		dmi.type = _htol16(dmi.type);
		dmi.size = _htol32(dmi.size);
		dmi.offset = _htol32(dmi.offset);
#ifdef DEBUG
		fprintf(stderr, "DEBUG: item %u, type 0x%x, size 0x%x@0x%x\n",
				i, dmi.type, dmi.size, dmi.offset);
#endif
		switch(dmi.type)
		{
			case TYPE_CODE_ITEM:
				ret |= _decode_map_code(format, i, dmi.offset,
						dmi.size);
				break;
			case TYPE_METHOD_ID_ITEM:
				ret |= _decode_map_method_id(format, dmi.offset,
						dmi.size);
				break;
			case TYPE_STRING_ID_ITEM:
				ret |= _decode_map_string_id(format, dmi.offset,
						dmi.size);
				break;
		}
		if(helper->seek(helper->format, offset, SEEK_SET) != offset)
			return -1;
		if(ret != 0)
			break;
	}
	return ret;
}

static int _decode_map_code(AsmFormatPlugin * format, size_t id, off_t offset,
		size_t size)
{
	AsmFormatPluginHelper * helper = format->helper;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%lu, %ld, %lu)\n", __func__, id, offset,
			size);
#endif
	return (helper->set_section(helper->format, id, ".text", offset, size,
				0) != NULL) ? 0 : -1;
}

static int _decode_map_method_id(AsmFormatPlugin * format, off_t offset,
		size_t size)
{
	AsmFormatPluginHelper * helper = format->helper;
	Dex * dex = format;
	ssize_t s;
	size_t i;
	AsmString * string;
	char const * name;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%ld, %lu)\n", __func__, offset, size);
#endif
	if(dex->dmii != NULL)
		return 0; /* already parsed */
	if(helper->seek(helper->format, offset, SEEK_SET) != offset)
		return -1;
	s = sizeof(*dex->dmii) * size;
	if((dex->dmii = malloc(s)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	if(helper->read(helper->format, dex->dmii, s) != s)
		return -1;
	for(i = 0; i < size; i++)
	{
		dex->dmii[i].class_idx = _htol16(dex->dmii[i].class_idx);
		dex->dmii[i].proto_idx = _htol16(dex->dmii[i].proto_idx);
		dex->dmii[i].name_idx = _htol32(dex->dmii[i].name_idx);
		if((string = helper->get_string_by_id(helper->format,
						dex->dmii[i].name_idx)) != NULL)
			name = string->name;
		else
			/* XXX report error? */
			name = NULL;
		helper->set_function(helper->format, i, name, -1, -1);
	}
	dex->dmii_cnt = size;
	return 0;
}

static int _decode_map_string_id(AsmFormatPlugin * format, off_t offset,
		size_t size)
{
	AsmFormatPluginHelper * helper = format->helper;
	DexStringIdItem * dsii;
	ssize_t s;
	size_t i;
	uint8_t u8;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%ld, %lu)\n", __func__, offset, size);
#endif
	if(helper->seek(helper->format, offset, SEEK_SET) != offset)
		return -1;
	s = sizeof(*dsii) * size;
	if((dsii = malloc(s)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	if(helper->read(helper->format, dsii, s) != s)
		return -1;
	for(i = 0; i < size; i++)
	{
		dsii[i].string_data_off = _htol32(dsii[i].string_data_off);
		offset = dsii[i].string_data_off;
		if(helper->seek(helper->format, offset, SEEK_SET) != offset)
			break;
		if(helper->read(helper->format, &u8, sizeof(u8)) != sizeof(u8))
			break;
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() string %lu offset 0x%lx len %u\n",
				__func__, i, offset, u8);
#endif
		helper->set_string(helper->format, i, NULL, offset + 1, u8);
	}
	free(dsii);
	return (i == size) ? 0 : -1;
}


/* dex_decode_section */
static int _dex_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	DexMapCodeItem dmci;
	size_t i;
	off_t seek;
	AsmFunction * f;
	size_t j;
	DexMapTryItem dmti;
	ssize_t s;
	uint32_t u32;
	int32_t s32;
	uint32_t v32;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(helper->seek(helper->format, section->offset, SEEK_SET)
			!= section->offset)
		return -1;
	for(i = 0; i < section->size; i++)
	{
		s = sizeof(dmci);
		if(helper->read(helper->format, &dmci, s) != s)
			return -1;
		dmci.registers_size = _htol16(dmci.registers_size);
		dmci.ins_size = _htol16(dmci.ins_size);
		dmci.outs_size = _htol16(dmci.outs_size);
		dmci.tries_size = _htol16(dmci.tries_size);
		dmci.debug_info_off = _htol32(dmci.debug_info_off);
		dmci.insns_size = _htol32(dmci.insns_size);
		seek = helper->seek(helper->format, 0, SEEK_CUR);
		if(helper->decode(helper->format, seek, dmci.insns_size * 2,
					seek, calls, calls_cnt) != 0)
			return -1;
		/* update the corresponding function offset */
		if((f = helper->get_function_by_id(helper->format, i)) != NULL)
			/* XXX not very optimal */
			helper->set_function(helper->format, i, f->name, seek,
					dmci.insns_size * 2);
		/* skip padding and try_items */
		seek = (dmci.insns_size & 0x1) == 0x1 ? 2 : 0;
#ifdef DEBUG
		fprintf(stderr, "DEBUG: code item %lu/%lu, offset 0x%lx"
				", registers 0x%x, size 0x%x, debug @0x%x"
				", tries 0x%x, seek 0x%lx\n", i, section->size,
				helper->seek(helper->format, 0, SEEK_CUR),
				dmci.registers_size, dmci.insns_size,
				dmci.debug_info_off, dmci.tries_size, seek);
#endif
		if(seek != 0 && helper->seek(helper->format, seek, SEEK_CUR)
				< 0)
			return -1;
		if(dmci.tries_size > 0)
		{
			for(j = 0; j < dmci.tries_size; j++)
			{
				s = sizeof(dmti);
				if(helper->read(helper->format, &dmti, s) != s)
					return -1;
				dmti.start_addr = _htol32(dmti.start_addr);
				dmti.insn_count = _htol16(dmti.insn_count);
				dmti.handler_off = _htol16(dmti.handler_off);
#ifdef DEBUG
				fprintf(stderr, "DEBUG: start 0x%x,"
						" insn_count 0x%x,"
						" handler_off 0x%x\n",
						dmti.start_addr,
						dmti.insn_count,
						dmti.handler_off);
#endif
			}
			/* encoded catch handler */
			/* list size */
			if(_dex_decode_uleb128(format, &u32) != 0)
				return -1;
			for(; u32 > 0; u32--)
			{
				/* handler size */
				if(_dex_decode_sleb128(format, &s32) != 0)
					return -1;
				/* address pairs */
				for(j = abs(s32); j > 0; j--)
				{
					if(_dex_decode_uleb128(format, &v32)
							!= 0)
						return -1;
					if(_dex_decode_uleb128(format, &v32)
							!= 0)
						return -1;
				}
				/* catch-all address */
				if(s32 <= 0 && _dex_decode_uleb128(format, &v32)
						!= 0)
					return -1;
			}
			/* ensure alignment on 4 bytes */
			seek = helper->seek(helper->format, 0, SEEK_CUR);
			if((seek = (4 - (seek & 0x3)) & 0x3) != 0)
				helper->seek(helper->format, seek, SEEK_CUR);
		}
	}
	return 0;
}


/* dex_decode_sleb128 */
static int _dex_decode_sleb128(AsmFormatPlugin * format, int32_t * s32)
{
	AsmFormatPluginHelper * helper = format->helper;
	uint32_t ret = 0;
	size_t i;
	unsigned char c;

	for(i = 0; i < 5; i++)
	{
		if(helper->read(helper->format, &c, sizeof(c)) != sizeof(c))
			return -1;
		ret |= ((c & 0x7f) << (7 * i));
		if((c & 0x80) != 0x80)
			/* FIXME properly sign-extend */
			break;
	}
	*s32 = ret;
	return 0;
}


/* dex_decode_uleb128 */
static int _dex_decode_uleb128(AsmFormatPlugin * format, uint32_t * u32)
{
	AsmFormatPluginHelper * helper = format->helper;
	uint32_t ret = 0;
	size_t i;
	unsigned char c;

	for(i = 0; i < 5; i++)
	{
		if(helper->read(helper->format, &c, sizeof(c)) != sizeof(c))
			return -1;
		ret |= ((c & 0x7f) << (7 * i));
		if((c & 0x80) != 0x80)
			break;
	}
	*u32 = ret;
	return 0;
}
