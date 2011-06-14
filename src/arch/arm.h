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

	switch(instruction->opcode & 0x0fffffff) /* ignore condition code */
	{
		case and:
		case eor:
		case sub:
		case rsb:
		case add:
		case adc:
		case sbc:
		case rsc:
		case orr:
		case bic:
		case and | (0x1 << 20):			/* ands */
		case eor | (0x1 << 20):			/* eors */
		case sub | (0x1 << 20):			/* subs */
		case rsb | (0x1 << 20):			/* rsbs */
		case add | (0x1 << 20):			/* adds */
		case adc | (0x1 << 20):			/* adcs */
		case sbc | (0x1 << 20):			/* sbcs */
		case rsc | (0x1 << 20):			/* rscs */
		case orr | (0x1 << 20):			/* orrs */
		case bic | (0x1 << 20):			/* bics */
			/* first operand, Rd */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			/* second operand, Rn */
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 16);
			/* third operand, Rm */
			p = call->operands[2].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			break;
		case and | (0x1 << 25):
		case eor | (0x1 << 25):
		case sub | (0x1 << 25):
		case rsb | (0x1 << 25):
		case add | (0x1 << 25):
		case adc | (0x1 << 25):
		case sbc | (0x1 << 25):
		case rsc | (0x1 << 25):
		case orr | (0x1 << 25):
		case bic | (0x1 << 25):
		case and | (0x1 << 20) | (0x1 << 25):
		case eor | (0x1 << 20) | (0x1 << 25):
		case sub | (0x1 << 20) | (0x1 << 25):
		case rsb | (0x1 << 20) | (0x1 << 25):
		case add | (0x1 << 20) | (0x1 << 25):
		case adc | (0x1 << 20) | (0x1 << 25):
		case sbc | (0x1 << 20) | (0x1 << 25):
		case rsc | (0x1 << 20) | (0x1 << 25):
		case orr | (0x1 << 20) | (0x1 << 25):
		case bic | (0x1 << 20) | (0x1 << 25):
			/* first operand, Rd */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			/* second operand, Rn */
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 16);
			/* third operand */
			opcode |= call->operands[2].value.immediate.value;
			break;
#if 1 /* FIXME implement */
		case tst:
		case teq:
		case cmp:
		case cmn:
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
			/* second operand */
			opcode |= call->operands[1].value.immediate.value;
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
