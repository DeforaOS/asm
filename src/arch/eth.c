/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
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


/* eth */
/* private */
/* variables */
static AsmArchDefinition const _eth_definition =
{
	"flat", ASM_ARCH_ENDIAN_BIG, 8, 8, 0
};

#define REG(name, size, id, flags, description) \
	{ "" # name, size, id, flags, description },
static AsmArchRegister const _eth_registers[] =
{
#include "null.reg"
};
#undef REG

static AsmArchInstruction const _eth_instructions[] =
{
#if 0
# include "eth.ins"
#endif
#include "common.ins"
#include "null.ins"
};

typedef struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
} EthArchPlugin;


/* prototypes */
/* plug-in */
static EthArchPlugin * _eth_init(AsmArchPluginHelper * helper);
static void _eth_destroy(EthArchPlugin * plugin);
static int _eth_decode(EthArchPlugin * plugin,
		AsmArchInstructionCall * call);
static int _eth_encode(EthArchPlugin * plugin,
		AsmArchPrefix const * prefix,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);


/* protected */
/* variables */
AsmArchPluginDefinition arch_plugin =
{
	"eth",
	"Ethereum",
	LICENSE_GNU_LGPL3_FLAGS,
	&_eth_definition,
	_eth_registers,
	NULL,
	_eth_instructions,
	_eth_init,
	_eth_destroy,
	_eth_encode,
	_eth_decode
};


/* functions */
/* plug-in */
/* eth_init */
static EthArchPlugin * _eth_init(AsmArchPluginHelper * helper)
{
	EthArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* eth_destroy */
static void _eth_destroy(EthArchPlugin * plugin)
{
	object_delete(plugin);
}


/* eth_decode */
static int _eth_decode(EthArchPlugin * plugin,
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


/* eth_encode */
static int _eth_encode(EthArchPlugin * plugin,
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
	opcode = instruction->opcode;
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}
