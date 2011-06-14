/* $Id$ */
/* Copyright (c) 2011 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel as */
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



#include <stddef.h>


/* arm */
/* private */
/* prototypes */
/* plug-in */
static int _arm_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


/* functions */
/* plug-in */
/* arm_write */
static int _arm_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t opcode = instruction->opcode;
	ArchRegister * ar;
	char const * p;

	switch(instruction->opcode & 0x0fffffff)
	{
#if 1 /* FIXME implement */
		case and:
		case eor:
		case sub:
		case rsb:
		case add:
		case adc:
		case sbc:
		case rsc:
		case tst:
		case teq:
		case cmp:
		case cmn:
		case orr:
		case bic:
			break;
#endif
		case mov:
		case mov | (0x1 << 20):			/* movs */
		case mvn:
		case mvn | (0x1 << 20):			/* mvns */
			if(call->operands_cnt == 0) /* nop */
				break;
			/* first operand, Rd */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			/* second operand, Rm */
			opcode |= (ar->id << 12);
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			break;
		case mov | (0x1 << 25):			/* mov (immediate) */
		case mov | (0x1 << 25) | (0x1 << 20):	/* movs (immediate) */
		case mvn | (0x1 << 25):			/* mvn (immediate) */
		case mvn | (0x1 << 25) | (0x1 << 20):	/* mvns (immediate) */
			if(call->operands_cnt == 0) /* nop */
				break;
			/* first operand, Rd */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			/* FIXME immediate value */
			break;
#if 1 /* FIXME really implement */
		default:
			break;
#endif
	}
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}
