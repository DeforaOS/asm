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
		/* branch and exchange */
		case OPBX(0):
			/* first operand, Rn */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			break;
		/* data processing */
		case OPDP(0, and):
		case OPDP(0, eor):
		case OPDP(0, sub):
		case OPDP(0, rsb):
		case OPDP(0, add):
		case OPDP(0, adc):
		case OPDP(0, sbc):
		case OPDP(0, rsc):
		case OPDP(0, orr):
		case OPDP(0, bic):
		case OPDPS(0, and):			/* ands */
		case OPDPS(0, eor):			/* eors */
		case OPDPS(0, sub):			/* subs */
		case OPDPS(0, rsb):			/* rsbs */
		case OPDPS(0, add):			/* adds */
		case OPDPS(0, adc):			/* adcs */
		case OPDPS(0, sbc):			/* sbcs */
		case OPDPS(0, rsc):			/* rscs */
		case OPDPS(0, orr):			/* orrs */
		case OPDPS(0, bic):			/* bics */
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
		case OPDPI(0, and):
		case OPDPI(0, eor):
		case OPDPI(0, sub):
		case OPDPI(0, rsb):
		case OPDPI(0, add):
		case OPDPI(0, adc):
		case OPDPI(0, sbc):
		case OPDPI(0, rsc):
		case OPDPI(0, orr):
		case OPDPI(0, bic):
		case OPDPIS(0, and):			/* ands (immediate) */
		case OPDPIS(0, eor):			/* eors (immediate) */
		case OPDPIS(0, sub):			/* subs (immediate) */
		case OPDPIS(0, rsb):			/* rsbs (immediate) */
		case OPDPIS(0, add):			/* adds (immediate) */
		case OPDPIS(0, adc):			/* adcs (immediate) */
		case OPDPIS(0, sbc):			/* sbcs (immediate) */
		case OPDPIS(0, rsc):			/* rscs (immediate) */
		case OPDPIS(0, orr):			/* orrs (immediate) */
		case OPDPIS(0, bic):			/* bics (immediate) */
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
		case OPDP(0, tst):
		case OPDP(0, teq):
		case OPDP(0, cmp):
		case OPDP(0, cmn):
		case OPDPS(0, tst):			/* tsts */
		case OPDPS(0, teq):			/* teqs */
		case OPDPS(0, cmp):			/* cmps */
		case OPDPS(0, cmn):			/* cmns */
			/* first operand, Rn */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 16);
			/* second operand, Rm */
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			break;
		case OPDPI(0, tst):
		case OPDPI(0, teq):
		case OPDPI(0, cmp):
		case OPDPI(0, cmn):
		case OPDPIS(0, tst):			/* tsts (immediate) */
		case OPDPIS(0, teq):			/* teqs (immediate) */
		case OPDPIS(0, cmp):			/* cmps (immediate) */
		case OPDPIS(0, cmn):			/* cmns (immediate) */
			/* first operand, Rn */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 16);
			/* second operand */
			opcode |= call->operands[1].value.immediate.value;
			break;
		case OPDP(0, mov):
		case OPDPS(0, mov):			/* movs */
		case OPDP(0, mvn):
		case OPDPS(0, mvn):			/* mvns */
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
		case OPDPI(0, mov):			/* mov (immediate) */
		case OPDPIS(0, mov):			/* movs (immediate) */
		case OPDPI(0, mvn):			/* mvn (immediate) */
		case OPDPIS(0, mvn):			/* mvns (immediate) */
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
