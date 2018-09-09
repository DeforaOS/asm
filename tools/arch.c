/* $Id$ */
/* Copyright (c) 2012-2018 Pierre Pronchery <khorben@defora.org> */
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



#include "../src/arch.c"


/* AsmArch */
/* private */
/* constants */
extern AsmArchPluginDefinition arch_plugin_amd64;
extern AsmArchPluginDefinition arch_plugin_arm;
extern AsmArchPluginDefinition arch_plugin_armeb;
extern AsmArchPluginDefinition arch_plugin_armel;
extern AsmArchPluginDefinition arch_plugin_dalvik;
extern AsmArchPluginDefinition arch_plugin_eth;
extern AsmArchPluginDefinition arch_plugin_i386;
extern AsmArchPluginDefinition arch_plugin_i386_real;
extern AsmArchPluginDefinition arch_plugin_i486;
extern AsmArchPluginDefinition arch_plugin_i586;
extern AsmArchPluginDefinition arch_plugin_i686;
extern AsmArchPluginDefinition arch_plugin_java;
extern AsmArchPluginDefinition arch_plugin_mips;
extern AsmArchPluginDefinition arch_plugin_mipseb;
extern AsmArchPluginDefinition arch_plugin_mipsel;
extern AsmArchPluginDefinition arch_plugin_sparc;
extern AsmArchPluginDefinition arch_plugin_sparc64;
extern AsmArchPluginDefinition arch_plugin_template;
extern AsmArchPluginDefinition arch_plugin_yasep;
extern AsmArchPluginDefinition arch_plugin_yasep16;
extern AsmArchPluginDefinition arch_plugin_yasep32;

static struct
{
	char const * name;
	AsmArchPluginDefinition * definition;
} _arch[] =
{
	{ "amd64",	NULL	},
	{ "arm",	NULL	},
	{ "armeb",	NULL	},
	{ "armel",	NULL	},
	{ "dalvik",	NULL	},
	{ "eth",	NULL	},
	{ "i386",	NULL	},
	{ "i386_real",	NULL	},
	{ "i486",	NULL	},
	{ "i586",	NULL	},
	{ "i686",	NULL	},
	{ "java",	NULL	},
	{ "mips",	NULL	},
	{ "mipseb",	NULL	},
	{ "mipsel",	NULL	},
	{ "sparc",	NULL	},
	{ "sparc64",	NULL	},
	{ "template",	NULL	},
	{ "yasep",	NULL	},
	{ "yasep16",	NULL	},
	{ "yasep32",	NULL	}
};


/* public */
/* functions */
/* arch_new */
AsmArch * arch_new(char const * name)
{
	AsmArch * a;
	AsmArchPluginDefinition * definition = NULL;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, name);
#endif
	/* XXX */
	if(_arch[0].definition == NULL)
	{
		_arch[0].definition = &arch_plugin_amd64;
		_arch[1].definition = &arch_plugin_arm;
		_arch[2].definition = &arch_plugin_armeb;
		_arch[3].definition = &arch_plugin_armel;
		_arch[4].definition = &arch_plugin_dalvik;
		_arch[5].definition = &arch_plugin_eth;
		_arch[6].definition = &arch_plugin_i386;
		_arch[7].definition = &arch_plugin_i386_real;
		_arch[8].definition = &arch_plugin_i486;
		_arch[9].definition = &arch_plugin_i586;
		_arch[10].definition = &arch_plugin_i686;
		_arch[11].definition = &arch_plugin_java;
		_arch[12].definition = &arch_plugin_mips;
		_arch[13].definition = &arch_plugin_mipseb;
		_arch[14].definition = &arch_plugin_mipsel;
		_arch[15].definition = &arch_plugin_sparc;
		_arch[16].definition = &arch_plugin_sparc64;
		_arch[17].definition = &arch_plugin_template;
		_arch[18].definition = &arch_plugin_yasep;
		_arch[19].definition = &arch_plugin_yasep16;
		_arch[20].definition = &arch_plugin_yasep32;
	}
	for(i = 0; i < sizeof(_arch) / sizeof(*_arch); i++)
		if(strcmp(_arch[i].name, name) == 0)
		{
			definition = _arch[i].definition;
			break;
		}
	if(definition == NULL)
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
	a->definition = definition;
	a->instructions_cnt = 0;
	if(a->definition->instructions != NULL)
		for(; a->definition->instructions[a->instructions_cnt].name
				!= NULL; a->instructions_cnt++);
	a->prefixes_cnt = 0;
	if(a->definition->prefixes != NULL)
		for(; a->definition->prefixes[a->prefixes_cnt].name != NULL;
				a->prefixes_cnt++);
	a->registers_cnt = 0;
	if(a->definition->registers != NULL)
		for(; a->definition->registers[a->registers_cnt].name != NULL;
				a->registers_cnt++);
	a->filename = NULL;
	a->fp = NULL;
	a->buffer = NULL;
	a->buffer_cnt = 0;
	a->buffer_pos = 0;
	return a;
}
