/* $Id$ */
/* Copyright (c) 2012 Pierre Pronchery <khorben@defora.org> */
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



#define arch_plugin arch_plugin_amd64
#include "../src/arch/amd64.c"
#undef arch_plugin

#define arch_plugin arch_plugin_dalvik
#include "../src/arch/dalvik.c"
#undef arch_plugin

#define arch_plugin arch_plugin_java
#include "../src/arch/java.c"
#undef arch_plugin

#define arch_plugin arch_plugin_mips
#include "../src/arch/mips.c"
#undef arch_plugin

#define arch_plugin arch_plugin_sparc
#include "../src/arch/sparc.c"
#undef arch_plugin

#define arch_plugin arch_plugin_sparc64
#include "../src/arch/sparc64.c"
#undef arch_plugin

#define arch_plugin arch_plugin_yasep
#include "../src/arch/yasep.c"
#undef arch_plugin

#if 0
#define arch_plugin arch_plugin_yasep16
#include "../src/arch/yasep16.c"
#undef arch_plugin

#define arch_plugin arch_plugin_yasep32
#include "../src/arch/yasep32.c"
#undef arch_plugin
#endif

#include "../src/arch.c"


/* AsmArch */
/* private */
/* constants */
extern AsmArchPlugin arch_plugin_arm;
extern AsmArchPlugin arch_plugin_armeb;
extern AsmArchPlugin arch_plugin_armel;
extern AsmArchPlugin arch_plugin_i386;
extern AsmArchPlugin arch_plugin_i486;
extern AsmArchPlugin arch_plugin_i586;
extern AsmArchPlugin arch_plugin_i686;
extern AsmArchPlugin arch_plugin_mipseb;
extern AsmArchPlugin arch_plugin_mipsel;

static struct
{
	char const * name;
	AsmArchPlugin * plugin;
} _arch[] =
{
	{ "amd64",	&arch_plugin_amd64	},
	{ "arm",	NULL			},
	{ "armeb",	NULL			},
	{ "armel",	NULL			},
	{ "dalvik",	&arch_plugin_dalvik	},
	{ "i386",	NULL			},
	{ "i486",	NULL			},
	{ "i586",	NULL			},
	{ "i686",	NULL			},
	{ "java",	&arch_plugin_java	},
	{ "mips",	&arch_plugin_mips	},
	{ "mipseb",	NULL			},
	{ "mipsel",	NULL			},
	{ "sparc",	&arch_plugin_sparc	},
	{ "sparc64",	&arch_plugin_sparc64	},
	{ "yasep",	&arch_plugin_yasep	}
};


/* public */
/* functions */
/* arch_new */
AsmArch * arch_new(char const * name)
{
	AsmArch * a;
	AsmArchPlugin * plugin = NULL;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	/* XXX */
	_arch[1].plugin = &arch_plugin_arm;
	_arch[2].plugin = &arch_plugin_armeb;
	_arch[3].plugin = &arch_plugin_armel;
	_arch[5].plugin = &arch_plugin_i386;
	_arch[6].plugin = &arch_plugin_i486;
	_arch[7].plugin = &arch_plugin_i586;
	_arch[8].plugin = &arch_plugin_i686;
	_arch[10].plugin = &arch_plugin_mipseb;
	_arch[11].plugin = &arch_plugin_mipsel;
	for(i = 0; i < sizeof(_arch) / sizeof(*_arch); i++)
		if(strcmp(_arch[i].name, name) == 0)
		{
			plugin = _arch[i].plugin;
			break;
		}
	if(plugin == NULL)
	{
		error_set_code(1, "%s: %s", name, "Unsupported architecture");
		return NULL;
	}
	if((a = object_new(sizeof(*a))) == NULL)
	{
		object_delete(a);
		return NULL;
	}
	memset(&a->helper, 0, sizeof(a->helper));
	a->handle = NULL;
	a->plugin = plugin;
	a->instructions_cnt = 0;
	if(a->plugin->instructions != NULL)
		for(; a->plugin->instructions[a->instructions_cnt].name != NULL;
				a->instructions_cnt++);
	a->registers_cnt = 0;
	if(a->plugin->registers != NULL)
		for(; a->plugin->registers[a->registers_cnt].name != NULL;
				a->registers_cnt++);
	a->filename = NULL;
	a->fp = NULL;
	a->buffer = NULL;
	a->buffer_cnt = 0;
	a->buffer_pos = 0;
	return a;
}
