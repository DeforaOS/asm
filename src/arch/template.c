/* $Id$ */
/* Copyright (c) 2014-2018 Pierre Pronchery <khorben@defora.org> */
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



#include <stddef.h>
#include <System.h>
#include "Asm.h"


/* template */
/* private */
/* variables */
static AsmArchDefinition const _template_definition =
{
	"flat", ASM_ARCH_ENDIAN_BOTH, 8, 8, 8
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _template_registers[] =
{
#include "null.reg"
};
#undef REG

static AsmArchPrefix const _template_prefixes[] =
{
#if 0
# include "template.pre"
#endif
#include "null.pre"
};

static AsmArchInstruction const _template_instructions[] =
{
#if 0
# include "template.ins"
#endif
#include "common.ins"
#include "null.ins"
};

struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
};


/* prototypes */
/* plug-in */
static AsmArchPlugin * _template_init(AsmArchPluginHelper * helper);
static void _template_destroy(AsmArchPlugin * plugin);
static int _template_decode(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call);
static int _template_encode(AsmArchPlugin * plugin,
		AsmArchPrefix const * prefix,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);


/* protected */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	"template",
	"Template",
	LICENSE_GNU_LGPL3_FLAGS,
	&_template_definition,
	_template_registers,
	_template_prefixes,
	_template_instructions,
	_template_init,
	_template_destroy,
	_template_encode,
	_template_decode
};


/* functions */
/* plug-in */
/* template_init */
static AsmArchPlugin * _template_init(AsmArchPluginHelper * helper)
{
	AsmArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* template_destroy */
static void _template_destroy(AsmArchPlugin * plugin)
{
	object_delete(plugin);
}


/* template_decode */
static int _template_decode(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint8_t u8;
	AsmArchInstruction const * ai;

	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 8, u8))
			== NULL)
		return -1;
	call->name = ai->name;
	call->operands[0].definition = ai->op1;
	call->operands[0].value.immediate.value = u8;
	call->operands_cnt = 1;
	return 0;
}


/* template_encode */
static int _template_encode(AsmArchPlugin * plugin,
		AsmArchPrefix const * prefix,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint8_t opcode;

	if(prefix != NULL)
		return -error_set_code(1, "%s: %s",
				helper->get_filename(helper->arch),
				"Prefixes not supported for this architecture");
	opcode = call->operands[0].value.immediate.value;
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}
