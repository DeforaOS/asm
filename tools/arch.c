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

#if 0
#define arch_plugin arch_plugin_arm
#include "../src/arch/arm.c"
#undef arch_plugin
#endif

#if 0
#define arch_plugin arch_plugin_armeb
#include "../src/arch/armeb.c"
#undef arch_plugin

#define arch_plugin arch_plugin_armel
#include "../src/arch/armel.c"
#undef arch_plugin
#endif

#define arch_plugin arch_plugin_dalvik
#include "../src/arch/dalvik.c"
#undef arch_plugin

#if 0
#define arch_plugin arch_plugin_i386
#include "../src/arch/i386.c"
#undef arch_plugin

#define arch_plugin arch_plugin_i386_real
#include "../src/arch/i386_real.c"
#undef arch_plugin

#define arch_plugin arch_plugin_i486
#include "../src/arch/i486.c"
#undef arch_plugin

#define arch_plugin arch_plugin_i586
#include "../src/arch/i586.c"
#undef arch_plugin

#define arch_plugin arch_plugin_i686
#include "../src/arch/i686.c"
#undef arch_plugin
#endif

#define arch_plugin arch_plugin_java
#include "../src/arch/java.c"
#undef arch_plugin

#define arch_plugin arch_plugin_mips
#include "../src/arch/mips.c"
#undef arch_plugin

#if 0
#define arch_plugin arch_plugin_mipseb
#include "../src/arch/mipseb.c"
#undef arch_plugin

#define arch_plugin arch_plugin_mipsel
#include "../src/arch/mipsel.c"
#undef arch_plugin
#endif

#define arch_plugin arch_plugin_sparc
#include "../src/arch/sparc.c"
#undef arch_plugin

#if 0
#define arch_plugin arch_plugin_sparc64
#include "../src/arch/sparc64.c"
#undef arch_plugin
#endif

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
static const struct
{
	char const * name;
	AsmArchPlugin * plugin;
} _arch[] =
{
	{ "amd64",	&arch_plugin_amd64	},
	{ "dalvik",	&arch_plugin_dalvik	},
	{ "java",	&arch_plugin_java	},
	{ "mips",	&arch_plugin_mips	},
	{ "sparc",	&arch_plugin_sparc	},
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
