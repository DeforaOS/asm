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
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Asm/as.h"
#include "Asm/format.h"
#include "arch.h"
#include "asm.h"
#include "../config.h"

#define min(a, b) ((a) < (b) ? (a) : (b))


/* disas */
/* private */
/* types */
typedef struct _DisasFormat
{
	Plugin * plugin;
	FormatPlugin * format;
} DisasFormat;

typedef struct _Disas
{
	FormatPluginHelper helper;
	char const * arch;
	As * as;

	DisasFormat * format;
	size_t format_cnt;
} Disas;


/* prototypes */
static int _disas(char const * arch, char const * format,
		char const * filename);
static int _disas_buffer(char const * arch, char const * format,
		char const * buffer, size_t size);
static int _disas_string(char const * arch, char const * format,
		char const * string);
static int _disas_list(void);

static int _disas_error(char const * message, int ret);

/* format plug-ins */
static DisasFormat const * _disas_format_open(Disas * disas,
		char const * format);
static int _disas_format_open_all(Disas * disas);
static int _disas_format_init(DisasFormat const * format, char const * arch);
static int _disas_format_exit(DisasFormat const * format);
static void _disas_format_close(DisasFormat const * format);
static void _disas_format_close_all(Disas * disas);

/* callbacks */
static int _disas_format_callback(FormatPlugin * format, char const * section,
		off_t offset, size_t size, off_t base);

/* helpers */
static int _hex2bin(int c);
static int _ishex(int c);


/* functions */
/* disas */
static int _disas_do_format(Disas * disas, char const * format);
static int _disas_do(Disas * disas);
static int _do_callback(Disas * disas, FormatPlugin * format);
static int _do_flat(Disas * disas, off_t offset, size_t size, off_t base);
static void _do_flat_print(Arch * arch, unsigned long address,
		char const * buffer, size_t size, ArchInstruction * ai);

static int _disas(char const * arch, char const * format, char const * filename)
{
	int ret = 1;
	Disas disas;
	size_t i;

	memset(&disas.helper, 0, sizeof(disas.helper));
	disas.arch = arch;
	disas.as = NULL;
	disas.format = NULL;
	disas.format_cnt = 0;
	if((disas.helper.fp = fopen(filename, "r")) == NULL)
		return -_disas_error(filename, 1);
	disas.helper.filename = filename;
	disas.helper.priv = &disas;
	if(format != NULL)
		ret = _disas_do_format(&disas, format);
	else
		ret = _disas_do(&disas);
	for(i = 0; i < disas.format_cnt; i++)
		_disas_format_close(&disas.format[i]);
	if(disas.as != NULL)
		as_delete(disas.as);
	fclose(disas.helper.fp);
	if(ret != 0)
		error_print("disas");
	return ret;
}

static int _disas_do_format(Disas * disas, char const * format)
{
	int ret;
	DisasFormat const * df;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, format);
#endif
	if((df = _disas_format_open(disas, format)) == NULL)
		return -1;
	if(df->format->disas == NULL)
		ret = -error_set_code(1, "%s: %s", format,
				"Does not support disassembly");
	else if((ret = _disas_format_init(df, disas->arch)) == 0)
	{
		ret = _do_callback(disas, df->format);
		_disas_format_exit(df);
	}
	_disas_format_close(df);
	return ret;
}

static int _disas_do(Disas * disas)
{
	int ret = 1;
	size_t i;
	size_t s = 0;
	char * buf;
	FormatPlugin * format;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	_disas_format_open_all(disas);
	for(i = 0; i < disas->format_cnt; i++)
		if(disas->format[i].format->signature_len > s)
			s = disas->format[i].format->signature_len;
	if((buf = malloc(s)) == NULL)
	{
		free(buf);
		return -_disas_error(disas->helper.filename, 1);
	}
	if(fread(buf, sizeof(*buf), s, disas->helper.fp) != s)
	{
		if(ferror(disas->helper.fp))
			ret = -_disas_error(disas->helper.filename, 1);
		else
			ret = _disas_do_format(disas, "flat");
	}
	else
	{
		for(i = 0; i < disas->format_cnt; i++)
		{
			format = disas->format[i].format;
			if(format->signature_len == 0)
				continue;
			else if(memcmp(format->signature, buf,
						format->signature_len) == 0)
			{
				if((ret = _disas_format_init(&disas->format[i],
								disas->arch))
						!= 0)
					break;
				ret = _do_callback(disas, format);
				_disas_format_exit(&disas->format[i]);
				break;
			}
		}
		if(i == disas->format_cnt)
			/* FIXME look it up in the existing list instead */
			ret = _disas_do_format(disas, "flat");
	}
	free(buf);
	_disas_format_close_all(disas);
	return ret;
}

static int _do_callback(Disas * disas, FormatPlugin * format)
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(disas->arch == NULL)
	{
		if(format->detect == NULL)
			return -error_set_code(1, "%s: %s", format->name,
					"Unable to detect the architecture");
		if((disas->arch = format->detect(format)) == NULL)
			return -1;
	}
	if((disas->as = as_new(disas->arch, format->name)) == NULL)
		return -error_print("disas");
	printf("\n%s: %s-%s\n", disas->helper.filename, format->name,
			as_get_arch_name(disas->as));
	ret = format->disas(format, _disas_format_callback);
	return ret;
}

static int _do_flat(Disas * disas, off_t offset, size_t size, off_t base)
{
	int ret = 0;
	Arch * arch = as_get_arch(disas->as);
	size_t pos;
	char buf[8];
	size_t buf_cnt = 0;
	size_t cnt;
	ArchInstruction * ai;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p, 0x%lx, 0x%lx) arch=\"%s\"\n", __func__,
			(void *)disas, (unsigned long)offset, size,
			as_get_arch_name(disas->as));
#endif
	if(fseek(disas->helper.fp, offset, SEEK_SET) != 0)
		return -_disas_error(disas->helper.filename, 1);
	printf("\n%08lx:\n", (unsigned long)offset + base);
	memset(buf, 0, sizeof(buf));
	for(pos = 0; pos < size; pos += cnt)
	{
		if((cnt = sizeof(buf) - buf_cnt) > 0 && size - pos - 1 > 0)
		{
			if((cnt = fread(&buf[buf_cnt], 1, cnt,
							disas->helper.fp)) == 0)
				break;
			buf_cnt += cnt;
		}
		cnt = buf_cnt;
		if((ai = as_decode(disas->as, buf, &cnt)) != NULL)
			_do_flat_print(arch, base + offset + pos, buf, cnt, ai);
		else
			cnt = 1; /* FIXME print missing instruction */
		memmove(buf, &buf[cnt], buf_cnt - cnt);
		buf_cnt -= cnt;
	}
	if(ferror(disas->helper.fp))
		return -_disas_error(disas->helper.filename, 1);
	else if(fseek(disas->helper.fp, offset + size, SEEK_SET) != 0)
		return -_disas_error(disas->helper.filename, 1);
	return ret;
}

static void _do_flat_print(Arch * arch, unsigned long address,
		char const * buffer, size_t size, ArchInstruction * ai)
{
	size_t pos = ai->size;
	size_t i;
	int col;
	ArchOperands operands;
	unsigned int reg;
	ArchRegister * ar;
	char const * sep = " ";
	unsigned long u;
	size_t j;
	size_t s;

	col = printf(" %5lx:", address);
	for(i = 0; i < size; i++)
		col += printf(" %02x", (unsigned char)buffer[i]);
	for(; col < 31; col++)
		putchar(' ');
	printf(" %s", ai->name);
	for(i = 0, operands = ai->operands; operands > 0; i++, operands >>= 8)
	{
		s = (i == 0) ? ai->op1size : ((i == 1) ? ai->op2size
				: ai->op3size);
		if((operands & _AO_OP) == _AO_REG && s == 0)
		{
			reg = (operands & 0xff) >> 2;
			if((ar = arch_register_get_by_id(arch, reg))
					!= NULL)
				printf("%s%%%s", sep, ar->name);
			else
				printf("%s%d", sep, reg);
			sep = ", ";
		}
		else if((operands & _AO_OP) == _AO_REG)
		{
			for(j = 0, u = 0; j < s; j++)
				u = (u << 8) | (unsigned char)buffer[pos++];
			/* XXX fix endian */
			if((ar = arch_register_get_by_id(arch, u)) != NULL)
				printf("%s%%%s", sep, ar->name);
			else
				printf("%s%lu", sep, u);
			sep = ", ";
		}
		else if((operands & _AO_OP) == _AO_DREG)
		{
			reg = (operands & 0xff) >> 2;
			if((ar = arch_register_get_by_id(arch, reg))
					!= NULL)
				printf("%s(%%%s)", sep, ar->name);
			else
				printf("%s(%d)", sep, reg);
			sep = ", ";
		}
		else if((operands & _AO_OP) == _AO_IMM)
		{
			for(j = 0, u = 0; j < s; j++)
				u = (u << 8) | (unsigned char)buffer[pos++];
			/* XXX fix endian */
			printf("%s$0x%lx", sep, u);
			sep = ", ";
		}
	}
	putchar('\n');
}


/* disas_buffer */
static int _disas_buffer(char const * arch, char const * format,
		char const * buffer, size_t size)
{
	As * as;
	Arch * a;
	size_t pos;
	size_t cnt;
	ArchInstruction * ai;

	if((as = as_new(arch, format)) == NULL)
		return -1;
	if((a = as_get_arch(as)) == NULL)
	{
		as_delete(as);
		return -1;
	}
	for(pos = 0; pos < size; pos += cnt)
	{
		cnt = size - pos;
		if((ai = as_decode(as, &buffer[pos], &cnt)) != NULL)
			_do_flat_print(a, pos, &buffer[pos], cnt, ai);
		else
			cnt = 1;
	}
	as_delete(as);
	return 0;
}


/* disas_string */
static int _disas_string(char const * arch, char const * format,
		char const * string)
{
	int ret;
	unsigned char * str = (unsigned char *)string;
	size_t len = strlen(string);
	char * s;
	size_t i;
	size_t j;

	if((s = malloc(len + 1)) == NULL)
		return -_disas_error("string", 1);
	for(i = 0, j = 0; i < len; i++)
	{
		if(str[i] != '\\')
			s[j++] = str[i];
		else if(str[i + 1] != 'x') /* "\\" */
			s[j++] = str[++i];
		else if(i + 3 < len && _ishex(str[i + 2])
				&& _ishex(str[i + 3])) /* "\xHH" */
		{
			s[j++] = (_hex2bin(str[i + 2]) << 4)
				| _hex2bin(str[i + 3]);
			i += 3;
		}
	}
	s[j] = '\0'; /* not really necessary */
	ret = _disas_buffer(arch, format, s, j);
	free(s);
	return ret;
}


/* disas_list */
static int _disas_list(void)
{
	Disas disas;
	size_t i;
	char const * sep = "";

	memset(&disas, 0, sizeof(disas));
	as_plugin_list(ASPT_ARCH);
	_disas_format_open_all(&disas);
	fputs("\nAvailable format plug-ins:\n", stderr);
	for(i = 0; i < disas.format_cnt; i++)
	{
		if(disas.format[i].format->disas == NULL)
			continue;
		fprintf(stderr, "%s%s", sep, disas.format[i].format->name);
		sep = ", ";
	}
	fputc('\n', stderr);
	_disas_format_close_all(&disas);
	return 0;
}


/* disas_error */
static int _disas_error(char const * message, int ret)
{
	fputs("disas: ", stderr);
	perror(message);
	return ret;
}


/* format plug-ins */
/* disas_format_open */
static DisasFormat const * _disas_format_open(Disas * disas,
		char const * format)
{
	DisasFormat * p;

	if((p = realloc(disas->format, sizeof(*p) * (disas->format_cnt + 1)))
			== NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		return NULL;
	}
	disas->format = p;
	p = &disas->format[disas->format_cnt];
	if((p->plugin = plugin_new(LIBDIR, PACKAGE, "format", format)) == NULL)
		return NULL;
	if((p->format = plugin_lookup(p->plugin, "format_plugin")) == NULL)
	{
		plugin_delete(p->plugin);
		return NULL;
	}
	p->format->helper = &disas->helper;
	return &disas->format[disas->format_cnt++];
}


/* disas_format_open_all */
static int _disas_format_open_all(Disas * disas)
{
	char const path[] = LIBDIR "/" PACKAGE "/format";
	DIR * dir;
	struct dirent * de;
	size_t len;
	char const ext[] = ".so";

	if((dir = opendir(path)) == NULL)
		return -error_set_print("disas", 1, "%s: %s", path, strerror(
					errno));
	while((de = readdir(dir)) != NULL)
	{
		if((len = strlen(de->d_name)) < 4)
			continue;
		if(strcmp(&de->d_name[len - sizeof(ext) + 1], ext) != 0)
			continue;
		de->d_name[len - sizeof(ext) + 1] = '\0';
		_disas_format_open(disas, de->d_name);
	}
	closedir(dir);
	return 0;
}


/* disas_format_init */
static int _disas_format_init(DisasFormat const * format, char const * arch)
{
	if(format->format->init == NULL)
		return 0;
	return format->format->init(format->format, arch);
}


/* disas_format_exit */
static int _disas_format_exit(DisasFormat const * format)
{
	if(format->format->exit == NULL)
		return 0;
	return format->format->exit(format->format);
}


/* disas_format_close */
static void _disas_format_close(DisasFormat const * format)
{
	plugin_delete(format->plugin);
}


/* disas_format_close_all */
static void _disas_format_close_all(Disas * disas)
{
	size_t i;

	for(i = 0; i < disas->format_cnt; i++)
		_disas_format_close(&disas->format[i]);
	disas->format_cnt = 0;
}


/* callbacks */
static int _disas_format_callback(FormatPlugin * format, char const * section,
		off_t offset, size_t size, off_t base)
{
	Disas * disas = format->helper->priv;

	if(section != NULL)
		printf("\nDisassembly of section %s:\n", section);
	return _do_flat(disas, offset, size, base);
}


/* helpers */
/* hex2bin */
static int _hex2bin(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}


/* ishex */
static int _ishex(int c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
		|| (c >= 'A' || c <= 'F');
}


/* usage */
static int _usage(void)
{
	fputs("Usage: disas [-a arch][-f format] filename\n"
"       disas [-a arch][-f format] -s string\n"
"       disas -l\n", stderr);
	return 1;
}


/* main */
int main(int argc, char * argv[])
{
	int o;
	char const * arch = NULL;
	char const * format = NULL;
	char const * string = NULL;

	while((o = getopt(argc, argv, "a:f:ls:")) != -1)
		switch(o)
		{
			case 'a':
				arch = optarg;
				break;
			case 'f':
				format = optarg;
				break;
			case 'l':
				return _disas_list();
			case 's':
				string = optarg;
				break;
			default:
				return _usage();
		}
	if(optind == argc && string != NULL)
		return _disas_string(arch, format, string);
	else if(optind + 1 == argc && string == NULL)
		return (_disas(arch, format, argv[optind])) == 0 ? 0 : 2;
	else
		return _usage();
}
