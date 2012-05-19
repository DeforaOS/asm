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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "Asm.h"


/* Java */
/* private */
/* types */
#pragma pack(1)
typedef struct _JavaHeader
{
	uint32_t magic;
	uint16_t minor;
	uint16_t major;
	uint16_t cp_cnt;
} JavaHeader;

typedef enum _JavaCpInfoTag
{
	CONSTANT_Utf8 = 1,
	CONSTANT_Integer = 3,
	CONSTANT_Float = 4,
	CONSTANT_Long = 5,
	CONSTANT_Double = 6,
	CONSTANT_Class = 7,
	CONSTANT_String = 8,
	CONSTANT_Fieldref = 9,
	CONSTANT_Methodref = 10,
	CONSTANT_InterfaceMethodref = 11,
	CONSTANT_NameAndType = 12
} JavaCpInfoTag;

typedef struct _JavaCpInfo
{
	off_t offset;
	uint8_t tag;
	union {
		struct {
			uint16_t name;
		} _class;

		struct {
			uint64_t value;
		} _double, _long;

		struct {
			uint16_t _class;
			uint16_t name_type;
		} field, interface, method;

		struct {
			uint32_t value;
		} _float, _integer;

		struct {
			uint16_t name;
			uint16_t descriptor;
		} name_type;

		struct {
			uint16_t name;
		} _string;

		struct {
			uint16_t length;
		} utf8;
	} info;
} JavaCpInfo;

typedef struct _JavaHeader2
{
	uint16_t access;
	uint16_t this;
	uint16_t super;
	uint16_t interfaces_cnt;
} JavaHeader2;

typedef struct _JavaFieldInfo
{
	uint16_t access;
	uint16_t name;
	uint16_t descriptor;
	uint16_t attributes_cnt;
} JavaFieldInfo;

typedef struct _JavaAttributeInfo
{
	uint16_t name;
	uint32_t length;
} JavaAttributeInfo;

typedef struct _JavaMethodInfo
{
	uint16_t access;
	uint16_t name;
	uint16_t descriptor;
	uint16_t attributes_cnt;

} JavaMethodInfo;
#pragma pack()

typedef struct _JavaPlugin
{
	char * class_name;
	char * super_name;
	uint16_t access_flags;
	JavaCpInfo * constants;
	uint16_t constants_cnt;
	uint16_t interfaces_cnt;
	uint16_t fields_cnt;
	uint16_t methods_cnt;
	uint16_t attributes_cnt;
} JavaPlugin;


/* variables */
/* plug-in */
static char _java_signature[4] = "\xca\xfe\xba\xbe";


/* prototypes */
/* plug-in */
static int _java_init(AsmFormatPlugin * format, char const * arch);
static int _java_exit(AsmFormatPlugin * format);
static char const * _java_detect(AsmFormatPlugin * format);
static int _java_decode(AsmFormatPlugin * format, int raw);
static int _java_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);


/* public */
/* variables */
/* format_plugin */
AsmFormatPlugin format_plugin =
{
	NULL,
	"java",
	_java_signature,
	sizeof(_java_signature),
	_java_init,
	_java_exit,
	NULL,
	NULL,
	_java_detect,
	_java_decode,
	_java_decode_section,
	NULL
};


/* private */
/* functions */
/* java_init */
static int _java_init(AsmFormatPlugin * format, char const * arch)
{
	JavaPlugin * java;

	if(arch != NULL && strcmp(arch, "java") != 0)
		return error_set_code(1, "%s: %s", arch,
				"Unsupported architecture for java");
#if 0 /* FIXME move this where appropriate */
	memcpy(&jh.magic, format->signature, format->signature_len);
	jh.minor = _htob16(0);
	jh.major = _htob16(0x32); /* XXX choose a more appropriate version */
	jh.cp_cnt = _htob16(0);
	if(helper->write(helper->format, &jh, sizeof(jh)) != sizeof(jh))
		return -1;
#endif
	if((java = object_new(sizeof(*java))) == NULL)
		return -1;
	memset(java, 0, sizeof(*java));
	format->priv = java;
	return 0;
}


/* java_exit */
static int _exit_constant_pool(AsmFormatPlugin * format);
static int _exit_access_flags(AsmFormatPlugin * format);
static int _exit_class_name(AsmFormatPlugin * format);
static int _exit_super_name(AsmFormatPlugin * format);
static int _exit_interface_table(AsmFormatPlugin * format);
static int _exit_field_table(AsmFormatPlugin * format);
static int _exit_method_table(AsmFormatPlugin * format);
static int _exit_attribute_table(AsmFormatPlugin * format);

static int _java_exit(AsmFormatPlugin * format)
{
	int ret = 0;
	JavaPlugin * java = format->priv;

	if(_exit_constant_pool(format) != 0
			|| _exit_access_flags(format) != 0
			|| _exit_class_name(format) != 0
			|| _exit_super_name(format) != 0
			|| _exit_interface_table(format) != 0
			|| _exit_field_table(format) != 0
			|| _exit_method_table(format) != 0
			|| _exit_attribute_table(format) != 0)
		ret = 1;
	free(java->constants);
	free(java);
	return ret;
}

static int _exit_constant_pool(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	uint16_t cnt = _htob16(java->constants_cnt + 1);

	if(helper->write(helper->format, &cnt, sizeof(cnt)) != sizeof(cnt))
		return -1;
	/* XXX output the constants */
	return 0;
}

static int _exit_access_flags(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	uint16_t flags = _htob16(java->access_flags);

	if(helper->write(helper->format, &flags, sizeof(flags))
			!= sizeof(flags))
		return -1;
	return 0;
}

static int _exit_class_name(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	uint16_t index = _htob16(0);

	/* FIXME really implement */
	if(helper->write(helper->format, &index, sizeof(index))
			!= sizeof(index))
		return -1;
	return 0;
}

static int _exit_super_name(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	uint16_t index = _htob16(0);

	/* FIXME really implement */
	if(helper->write(helper->format, &index, sizeof(index))
			!= sizeof(index))
		return -1;
	return 0;
}

static int _exit_interface_table(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	uint16_t cnt = _htob16(java->interfaces_cnt);

	if(helper->write(helper->format, &cnt, sizeof(cnt)) != sizeof(cnt))
		return -1;
	/* XXX output the interfaces */
	return 0;
}

static int _exit_field_table(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	uint16_t cnt = _htob16(java->fields_cnt);

	if(helper->write(helper->format, &cnt, sizeof(cnt)) != sizeof(cnt))
		return -1;
	/* XXX output the fields */
	return 0;
}

static int _exit_method_table(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	uint16_t cnt = _htob16(java->methods_cnt);

	if(helper->write(helper->format, &cnt, sizeof(cnt)) != sizeof(cnt))
		return -1;
	/* XXX output the methods */
	return 0;
}

static int _exit_attribute_table(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	uint16_t cnt = _htob16(java->attributes_cnt);

	if(helper->write(helper->format, &cnt, sizeof(cnt)) != sizeof(cnt))
		return -1;
	/* XXX output the attributes */
	return 0;
}


/* java_detect */
static char const * _java_detect(AsmFormatPlugin * format)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaHeader jh;

	if(helper->seek(helper->format, 0, SEEK_SET) != 0)
		return NULL;
	if(helper->read(helper->format, &jh, sizeof(jh)) != sizeof(jh))
		return NULL;
	if(memcmp(&jh.magic, _java_signature, sizeof(jh.magic)) != 0)
		return NULL;
	jh.minor = _htob16(jh.minor);
	jh.major = _htob16(jh.major);
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %u.%d\n", __func__, jh.major, jh.minor);
#endif
	return "java";
}


/* java_decode */
static int _java_decode(AsmFormatPlugin * format, int raw)
{
	AsmFormatPluginHelper * helper = format->helper;
	off_t end;

	/* XXX consider the whole file as a section */
	if((end = helper->seek(helper->format, 0, SEEK_END)) < 0)
		return -1;
	return helper->set_section(helper->format, 0, ".text", 0, end, 0);
}


/* java_decode_section */
static int _decode_attributes(AsmFormatPlugin * format, uint16_t cnt,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);
static int _decode_constants(AsmFormatPlugin * format, uint16_t cnt);
static int _decode_fields(AsmFormatPlugin * format, uint16_t cnt);
static int _decode_interfaces(AsmFormatPlugin * format, uint16_t cnt);
static int _decode_methods(AsmFormatPlugin * format, uint16_t cnt,
		AsmArchInstructionCall ** calls, size_t * calls_cnt);
static int _methods_add(AsmFormatPlugin * format, uint16_t id, uint16_t name,
		off_t offset, size_t size);

static int _java_decode_section(AsmFormatPlugin * format, AsmSection * section,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	JavaHeader jh;
	JavaHeader2 jh2;
	uint16_t u16;

	/* reset the status */
	free(java->constants);
	java->constants = NULL;
	java->constants_cnt = 0;
	/* read header */
	if(helper->seek(helper->format, section->offset, SEEK_SET)
			!= section->offset)
		return -1;
	if(helper->read(helper->format, &jh, sizeof(jh)) != sizeof(jh))
		return -1;
	/* decode constants */
	jh.cp_cnt = _htob16(jh.cp_cnt);
	if(jh.cp_cnt > 1 && _decode_constants(format, jh.cp_cnt) != 0)
		return -1;
	/* skip interfaces */
	if(helper->read(helper->format, &jh2, sizeof(jh2)) != sizeof(jh2))
		return -1;
	jh2.interfaces_cnt = _htob16(jh2.interfaces_cnt);
	if(_decode_interfaces(format, jh2.interfaces_cnt) != 0)
		return -1;
	/* skip fields */
	if(helper->read(helper->format, &u16, sizeof(u16)) != sizeof(u16))
		return -1;
	u16 = _htob16(u16);
	if(_decode_fields(format, u16) != 0)
		return -1;
	/* decode methods */
	if(helper->read(helper->format, &u16, sizeof(u16)) != sizeof(u16))
		return -1;
	u16 = _htob16(u16);
	if(_decode_methods(format, u16, calls, calls_cnt) != 0)
		return -1;
	return 0;
}

static int _decode_attributes(AsmFormatPlugin * format, uint16_t cnt,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	size_t i;
	JavaAttributeInfo jai;
	off_t offset;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u)\n", __func__, cnt);
#endif
	for(i = 0; i < cnt; i++)
	{
		if(helper->read(helper->format, &jai, sizeof(jai))
				!= sizeof(jai))
			return -1;
		jai.length = _htob32(jai.length);
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() length=%u\n", __func__,
				jai.length);
#endif
		if(calls != NULL)
		{
			offset = helper->seek(helper->format, 0, SEEK_CUR);
			if(offset < 0 || helper->decode(helper->format, offset,
						jai.length, offset, calls,
						calls_cnt) != 0)
				return -1;
		}
		else if(helper->seek(helper->format, jai.length, SEEK_CUR) < 0)
				return -1;
	}
	return 0;
}

static int _decode_constants(AsmFormatPlugin * format, uint16_t cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	size_t i;
	JavaCpInfo * jci;
	size_t size;
	size_t name;
	JavaCpInfo * jcin;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u)\n", __func__, cnt);
#endif
	if((java->constants = malloc(sizeof(*java->constants) * cnt)) == NULL)
		return -error_set_code(1, "%s", strerror(errno));
	java->constants_cnt = cnt;
	/* zero all the constants out */
	memset(java->constants, 0, sizeof(*java->constants) * cnt);
	for(i = 1; i < cnt; i++)
	{
		jci = &java->constants[i];
		if((jci->offset = helper->seek(helper->format, 0, SEEK_CUR))
				< 0)
			return -1;
		if(helper->read(helper->format, &jci->tag, sizeof(jci->tag))
				!= sizeof(jci->tag))
			return -1;
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() %lu (%u)\n", __func__, i,
				jci->tag);
#endif
		switch(jci->tag)
		{
			case CONSTANT_Class:
			case CONSTANT_String:
				size = sizeof(jci->info._class);
				if(helper->read(helper->format,
							&jci->info._class,
							size) != (ssize_t)size)
					return -1;
				jci->info._class.name = _htob16(
						jci->info._class.name);
				break;
			case CONSTANT_Double:
			case CONSTANT_Long:
				size = sizeof(jci->info._double);
				if(helper->read(helper->format,
							&jci->info._double,
							size) != (ssize_t)size)
					return -1;
				jci->info._double.value = _htob64(
						jci->info._double.value);
				break;
			case CONSTANT_Fieldref:
			case CONSTANT_InterfaceMethodref:
			case CONSTANT_Methodref:
				size = sizeof(jci->info.field);
				if(helper->read(helper->format,
							&jci->info.field,
							size) != (ssize_t)size)
					return -1;
				jci->info.field._class = _htob16(
						jci->info.field._class);
				jci->info.field.name_type = _htob16(
						jci->info.field.name_type);
				break;
			case CONSTANT_Float:
			case CONSTANT_Integer:
				size = sizeof(jci->info._float);
				if(helper->read(helper->format,
							&jci->info._float,
							size) != (ssize_t)size)
					return -1;
				jci->info._float.value = _htob32(
						jci->info._float.value);
				break;
			case CONSTANT_NameAndType:
				size = sizeof(jci->info.name_type);
				if(helper->read(helper->format,
							&jci->info.name_type,
							size) != (ssize_t)size)
					return -1;
				jci->info.name_type.name = _htob16(
						jci->info.name_type.name);
				jci->info.name_type.descriptor = _htob16(
						jci->info.name_type.descriptor);
				break;
			case CONSTANT_Utf8:
				size = sizeof(jci->info.utf8);
				if(helper->read(helper->format, &jci->info.utf8,
							size) != (ssize_t)size)
					return -1;
				jci->info.utf8.length = _htob16(
						jci->info.utf8.length);
				/* skip the string */
				if(helper->seek(helper->format,
							jci->info.utf8.length,
							SEEK_CUR) < 0)
					return -1;
				break;
			default:
				return -error_set_code(1, "%s: %s 0x%x",
						helper->get_filename(
							helper->format),
						"Unknown constant tag",
						jci->tag);
		}
	}
	/* assign all the strings */
	for(i = 1; i < cnt; i++)
	{
		jci = &java->constants[i];
		switch(jci->tag)
		{
			case CONSTANT_Class:
			case CONSTANT_String:
				if((name = jci->info._class.name) >= cnt)
					continue;
				if(java->constants[name].tag != CONSTANT_Utf8)
					continue;
				jcin = &java->constants[name];
				if(helper->set_string(helper->format, i, NULL,
							jcin->offset + 3,
							jcin->info.utf8.length)
						< 0)
					return -1;
				break;
			case CONSTANT_Fieldref:
			case CONSTANT_InterfaceMethodref:
			case CONSTANT_Methodref:
				if((name = jci->info.field.name_type) >= cnt)
					continue;
				jcin = &java->constants[name];
				if(jcin->tag != CONSTANT_NameAndType)
					continue;
				if((name = jcin->info.name_type.descriptor)
						>= cnt)
					continue;
				jcin = &java->constants[name];
				if(jcin->tag != CONSTANT_Utf8)
					continue;
				if(helper->set_string(helper->format, i, NULL,
							jcin->offset + 3,
							jcin->info.utf8.length)
						< 0)
					return -1;
				break;
		}
	}
	return 0;
}

static int _decode_fields(AsmFormatPlugin * format, uint16_t cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	size_t i;
	JavaFieldInfo jfi;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u)\n", __func__, cnt);
#endif
	for(i = 0; i < cnt; i++)
	{
		if(helper->read(helper->format, &jfi, sizeof(jfi))
				!= sizeof(jfi))
			return -1;
		jfi.attributes_cnt = _htob16(jfi.attributes_cnt);
		_decode_attributes(format, jfi.attributes_cnt, NULL, NULL);
	}
	return 0;
}

static int _decode_interfaces(AsmFormatPlugin * format, uint16_t cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	size_t i;
	uint16_t u16;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u)\n", __func__, cnt);
#endif
	for(i = 0; i < cnt; i++)
		if(helper->read(helper->format, &u16, sizeof(u16))
				!= sizeof(u16))
			return -1;
	return 0;
}

static int _decode_methods(AsmFormatPlugin * format, uint16_t cnt,
		AsmArchInstructionCall ** calls, size_t * calls_cnt)
{
	AsmFormatPluginHelper * helper = format->helper;
	size_t i;
	JavaMethodInfo jmi;
	off_t begin;
	off_t end;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u)\n", __func__, cnt);
#endif
	for(i = 0; i < cnt; i++)
	{
		if(helper->read(helper->format, &jmi, sizeof(jmi))
				!= sizeof(jmi))
			return -1;
		jmi.access = _htob16(jmi.access);
		jmi.name = _htob16(jmi.name);
		jmi.descriptor = _htob16(jmi.descriptor);
		jmi.attributes_cnt = _htob16(jmi.attributes_cnt);
		/* decode attributes and code */
		if((begin = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
			return -1;
		if(jmi.attributes_cnt > 0)
			begin += sizeof(JavaAttributeInfo);
		_decode_attributes(format, jmi.attributes_cnt, calls,
				calls_cnt);
		if((end = helper->seek(helper->format, 0, SEEK_CUR)) < 0)
			return -1;
		/* add the method to the function list */
		_methods_add(format, i, jmi.name, begin, end - begin);
	}
	return 0;
}

static int _methods_add(AsmFormatPlugin * format, uint16_t id, uint16_t name,
		off_t offset, size_t size)
{
	AsmFormatPluginHelper * helper = format->helper;
	JavaPlugin * java = format->priv;
	JavaCpInfo * jci;
	AsmString * as;

	jci = &java->constants[name];
	if(name >= java->constants_cnt || jci->tag != CONSTANT_Utf8)
		return 0;
	if(helper->set_string(helper->format, name, NULL, jci->offset + 3,
				jci->info.utf8.length) < 0)
		return -1;
	if((as = helper->get_string_by_id(helper->format, name)) == NULL)
		return -1;
	return helper->set_function(helper->format, id, as->name, offset, size);
}
