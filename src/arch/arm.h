/* $Id$ */
/* Copyright (c) 2011-2012 Pierre Pronchery <khorben@defora.org> */
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



#include <System.h>


/* arm */
/* private */
/* types */
struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
};


/* prototypes */
/* plug-in */
static AsmArchPlugin * _arm_init(AsmArchPluginHelper * helper);
static void _arm_destroy(AsmArchPlugin * plugin);
static int _arm_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call);
static int _arm_encode(AsmArchPlugin * plugin, AsmArchInstruction * instruction,
		AsmArchInstructionCall * call);


/* functions */
/* plug-in */
/* arm_init */
static AsmArchPlugin * _arm_init(AsmArchPluginHelper * helper)
{
	AsmArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* arm_destroy */
static void _arm_destroy(AsmArchPlugin * plugin)
{
	object_delete(plugin);
}


/* arm_decode */
static void _decode_reg_reg_dreg(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode);
static void _decode_reg_reg_reg(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode);
static void _decode_reg_reg_u12(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode);
static void _decode_s24(AsmArchInstructionCall * call, uint32_t opcode);
static void _decode_u24(AsmArchInstructionCall * call, uint32_t opcode);
static void _decode_u4_u4_reg(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode);
static int _decode_unknown(AsmArchInstructionCall * call, uint32_t opcode);

static int _arm_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint32_t opcode;
	uint32_t op;
	AsmArchInstruction * ai;

	/* read 4 bytes in the proper endian */
	if(helper->read(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
#if 1
	/* FIXME apply as relevant */
	opcode = _htob32(opcode);
#endif
	/* lookup the instruction */
	/* FIXME decode everything in the proper order */
	/* opcode bits 27, 26, 25 and 24 set */
	if((op = (opcode & OPSI(0x0))) == OPSI(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSI(0xf));
		_decode_u24(call, opcode);
	}
	/* opcode bits 27, 26, 25 set */
	else if((op = (opcode & OPCDO(0x0))) == OPCDO(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPCDO(0xf));
		_decode_u4_u4_reg(plugin, call, opcode);
	}
	/* opcode bits 27, 25, 24 set */
	else if((op = (opcode & OPBL(0x0))) == OPBL(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPBL(0xf));
		_decode_s24(call, opcode);
	}
	/* opcode bits 27, 25 set */
	else if((op = (opcode & OPB(0x0))) == OPB(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPB(0xf));
		_decode_s24(call, opcode);
	}
	/* opcode bits 26, 25, 22, 20 */
	else if((op = (opcode & OPSDTLB(0x0))) == OPSDTLB(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSDTLB(0xf));
	/* opcode bits 26, 25, 22 set */
	else if((op = (opcode & OPSDTSB(0x0))) == OPSDTSB(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSDTSB(0xf));
	/* opcode bits 26, 25, 20 set */
	else if((op = (opcode & OPSDTL(0x0))) == OPSDTL(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSDTL(0xf));
	/* opcode bits 26, 25 set */
	else if((op = (opcode & OPSDTS(0x0))) == OPSDTS(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSDTS(0xf));
	/* opcode bits 25, 20 set */
	else if((op = (opcode & OPDPIS(0x0, 0x0))) == OPDPIS(0x0, 0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPDPIS(0xf, 0xf));
		_decode_reg_reg_u12(plugin, call, opcode);
	}
	/* bit 25 is set */
	else if((op = (opcode & OPDPI(0x0, 0x0))) == OPDPI(0x0, 0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPDPI(0xf, 0xf));
		_decode_reg_reg_u12(plugin, call, opcode);
	}
	/* opcode bits 24, 22 set */
	else if((op = (opcode & OPSDSB(0x0))) == OPSDSB(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSDSB(0xf));
		_decode_reg_reg_dreg(plugin, call, opcode);
	}
	/* bit 24 is set */
	else if((op = (opcode & OPPTI(0x0))) == OPPTI(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPPTI(0xf));
	/* bit 24 is set */
	else if((op = (opcode & OPPT(0x0))) == OPPT(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPPT(0xf));
	/* bit 24 is set */
	else if((op = (opcode & OPSDS(0x0))) == OPSDS(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPSDS(0xf));
		_decode_reg_reg_dreg(plugin, call, opcode);
	}
	/* opcode bits 21, 20, 8 and 4 set */
	else if((op = (opcode & OPMULAS(0x0))) == OPMULAS(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPMULAS(0xf));
	/* opcode bits 21, 18, 16-8, 4 set */
	else if((op = (opcode & OPBX(0x0))) == OPBX(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPBX(0xf));
	/* opcode bits 21, 8 and 4 set */
	else if((op = (opcode & OPMULA(0x0))) == OPMULA(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPMULA(0xf));
	/* opcode bits 20, 8 and 4 set */
	else if((op = (opcode & OPMULS(0x0))) == OPMULS(0x0))
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPMULS(0xf));
	/* opcode bit 20 set */
	else if((op = (opcode & OPDPS(0x0, 0x0))) == OPDPS(0x0, 0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPDPS(0xf, 0xf));
		_decode_reg_reg_reg(plugin, call, opcode);
	}
	/* opcode bits 8 and 4 set */
	else if((op = (opcode & OPMUL(0x0))) == OPMUL(0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPMUL(0xf));
		_decode_reg_reg_reg(plugin, call, opcode);
	}
	/* no opcode bits set */
	else if((op = (opcode & OPDP(0x0, 0x0))) == OPDP(0x0, 0x0))
	{
		ai = helper->get_instruction_by_opcode(helper->arch, 32,
				opcode & OPDP(0xf, 0xf));
		_decode_reg_reg_reg(plugin, call, opcode);
	}
	else
		/* unknown instruction */
		return _decode_unknown(call, opcode);
	call->name = ai->name;
	call->operands_cnt = 0;
	if((call->operands[0].definition = ai->op1) != AOT_NONE)
		call->operands_cnt++;
	if((call->operands[1].definition = ai->op2) != AOT_NONE)
		call->operands_cnt++;
	if((call->operands[2].definition = ai->op3) != AOT_NONE)
		call->operands_cnt++;
	return 0;
}

static void _decode_reg_reg_dreg(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchRegister * ar;

	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 12) & 0xf, 32)) != NULL)
		call->operands[0].value._register.name = ar->name;
	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 16) & 0xf, 32)) != NULL)
		call->operands[1].value._register.name = ar->name;
	if((ar = helper->get_register_by_id_size(helper->arch,
					opcode & 0xf, 32)) != NULL)
		call->operands[2].value.dregister.name = ar->name;
}

static void _decode_reg_reg_reg(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchRegister * ar;

	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 12) & 0xf, 32)) != NULL)
		call->operands[0].value._register.name = ar->name;
	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 16) & 0xf, 32)) != NULL)
		call->operands[1].value._register.name = ar->name;
	if((ar = helper->get_register_by_id_size(helper->arch,
					opcode & 0xf, 32)) != NULL)
		call->operands[2].value._register.name = ar->name;
}

static void _decode_reg_reg_u12(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchRegister * ar;

	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 12) & 0xf, 32)) != NULL)
		call->operands[0].value._register.name = ar->name;
	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 16) & 0xf, 32)) != NULL)
		call->operands[1].value._register.name = ar->name;
	call->operands[2].value.immediate.value = opcode & 0xfff;
}

static void _decode_s24(AsmArchInstructionCall * call, uint32_t opcode)
{
	call->operands[0].value.immediate.value = opcode & 0x00ffffff;
	/* FIXME properly restore the sign */
	if(opcode & 0x00800000)
		call->operands[0].value.immediate.negative = 1;
}

static void _decode_u24(AsmArchInstructionCall * call, uint32_t opcode)
{
	call->operands[0].value.immediate.value = opcode & 0x00ffffff;
}

static void _decode_u4_u4_reg(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, uint32_t opcode)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchRegister * ar;

	/* FIXME implement u4 and u4 */
	if((ar = helper->get_register_by_id_size(helper->arch,
					(opcode >> 12) & 0xf, 32)) != NULL)
		call->operands[2].value._register.name = ar->name;
}

static int _decode_unknown(AsmArchInstructionCall * call, uint32_t opcode)
{
	call->name = "dw";
	call->operands[0].definition = AO_IMMEDIATE(0, 32, 0);
	call->operands[0].value.immediate.value = opcode;
	call->operands_cnt = 1;
	return 0;
}


/* arm_encode */
static int _arm_encode(AsmArchPlugin * plugin, AsmArchInstruction * instruction,
		AsmArchInstructionCall * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	uint32_t opcode = instruction->opcode;
	AsmArchRegister * ar;
	char const * p;

	switch(opcode & 0x0fffffff) /* ignore condition code */
	{
		/* branch, branch with link */
		case OPB(0):				/* b */
		case OPBL(0):				/* bl */
			/* FIXME properly keep the sign */
			opcode |= (call->operands[0].value.immediate.value
					& 0x00ffffff);
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
#if 1
	/* FIXME apply as relevant */
	opcode = _htob32(opcode);
#endif
	if(helper->write(helper->arch, &opcode, sizeof(opcode))
			!= sizeof(opcode))
		return -1;
	return 0;
}
