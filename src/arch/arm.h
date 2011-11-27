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
static int _arm_encode(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


/* functions */
/* plug-in */
/* arm_encode */
static int _arm_encode(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	uint32_t opcode = instruction->opcode;
	ArchRegister * ar;
	char const * p;

	switch(instruction->opcode & 0x0fffffff) /* ignore condition code */
	{
		/* branch, branch with link */
		case OPB(0):				/* b */
		case OPBL(0):				/* bl */
			opcode |= call->operands[0].value.immediate.value;
			break;
		/* branch and exchange */
		case OPBX(0):				/* bx */
			/* first operand, Rn */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			break;
		/* data processing */
		case OPDP(0, and):			/* and */
		case OPDP(0, eor):			/* eor */
		case OPDP(0, sub):			/* sub */
		case OPDP(0, rsb):			/* rsb */
		case OPDP(0, add):			/* add */
		case OPDP(0, adc):			/* adc */
		case OPDP(0, sbc):			/* sbc */
		case OPDP(0, rsc):			/* rsc */
		case OPDP(0, orr):			/* orr */
		case OPDP(0, bic):			/* bic */
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
		case OPDPI(0, and):			/* and (immediate) */
		case OPDPI(0, eor):			/* eor (immediate) */
		case OPDPI(0, sub):			/* sub (immediate) */
		case OPDPI(0, rsb):			/* rsb (immediate) */
		case OPDPI(0, add):			/* add (immediate) */
		case OPDPI(0, adc):			/* adc (immediate) */
		case OPDPI(0, sbc):			/* sbc (immediate) */
		case OPDPI(0, rsc):			/* rsc (immediate) */
		case OPDPI(0, orr):			/* orr (immediate) */
		case OPDPI(0, bic):			/* bic (immediate) */
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
		case OPDP(0, tst):			/* tst */
		case OPDP(0, teq):			/* teq */
		case OPDP(0, cmp):			/* cmp */
		case OPDP(0, cmn):			/* cmn */
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
		case OPDPI(0, tst):			/* tst (immediate) */
		case OPDPI(0, teq):			/* teq (immediate) */
		case OPDPI(0, cmp):			/* cmp (immediate) */
		case OPDPI(0, cmn):			/* cmn (immediate) */
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
		case OPDP(0, mov):			/* mov */
		case OPDPS(0, mov):			/* movs */
		case OPDP(0, mvn):			/* mvn */
		case OPDPS(0, mvn):			/* mvns */
			/* take care of nop */
			if(call->operands_cnt == 0)
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
		/* psr transfer */
		case OPPT(0):
			/* first operand, Rd */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			break;
		case OPPTI(0):
			/* second operand, Rm */
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			break;
		/* multiply and multiply-accumulate */
		case OPMUL(0):				/* mul */
		case OPMULS(0):				/* muls */
		case OPMULA(0):				/* mla */
		case OPMULAS(0):			/* mlas */
			/* first operand, Rd */
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
			/* third operand, Rs */
			p = call->operands[2].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 8);
			break;
		/* single data transfer */
		case OPSDTL(0):				/* ldr */
		case OPSDTS(0):				/* str */
		case OPSDTLB(0):			/* ldrb */
		case OPSDTSB(0):			/* strb */
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
		/* block data transfer */
		case OPBDTL(0):				/* ldm */
		case OPBDTS(0):				/* stm */
			/* first operand, Rn */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 16);
			/* second operand, register list */
			opcode |= call->operands[1].value.immediate.value;
			break;
		/* single data swap */
		case OPSDS(0):
		case OPSDSB(0):
			/* first operand, Rd */
			p = call->operands[0].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			/* second operand, Rm */
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= ar->id;
			/* third operand, Rn */
			p = call->operands[2].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 16);
			break;
		/* software interrupt */
		case OPSI(0):
			opcode |= call->operands[0].value.immediate.value;
			break;
		/* coprocessor data operation */
		case OPCDO(0):
			/* first operand, coprocessor number */
			opcode |= (call->operands[0].value.immediate.value
					<< 8);
			/* second operand, coprocessor operation code */
			opcode |= (call->operands[1].value.immediate.value
					<< 20);
			/* third operand, CRd */
			p = call->operands[2].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			break;
		/* coprocessor data transfers */
		case OPCDTL(0):
		case OPCDTS(0):
			/* first operand, coprocessor number */
			opcode |= (call->operands[0].value.immediate.value
					<< 8);
			/* second operand, CRd */
			p = call->operands[1].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			/* third operand, address */
			opcode |= call->operands[2].value.immediate.value;
			break;
		/* coprocessor register transfers */
		case OPCRTL(0):
		case OPCRTS(0):
			/* first operand, coprocessor number */
			opcode |= (call->operands[0].value.immediate.value
					<< 8);
			/* second operand, opcode */
			opcode |= (call->operands[1].value.immediate.value
					<< 21);
			/* third operand, Rd */
			p = call->operands[2].value._register.name;
			if((ar = helper->get_register_by_name_size(helper->arch,
							p, 32)) == NULL)
				return -1;
			opcode |= (ar->id << 12);
			/* FIXME implement */
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
