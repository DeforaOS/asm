/* $Id$ */
/* Copyright (c) 2011-2017 Pierre Pronchery <khorben@defora.org> */
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




#ifndef ASM_ARCH_I386_H
# define ASM_ARCH_I386_H

# include <System.h>
# include <string.h>
# include <errno.h>


/* i386 */
/* private */
/* types */
struct _AsmArchPlugin
{
	AsmArchPluginHelper * helper;
};


/* constants */
extern AsmArchPluginDefinition arch_plugin;


/* prototypes */
/* plug-in */
static AsmArchPlugin * _i386_init(AsmArchPluginHelper * helper);
static void _i386_destroy(AsmArchPlugin * plugin);
static int _i386_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call);
static int _i386_encode(AsmArchPlugin * plugin,
		AsmArchPrefix const * prefix,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call);


/* functions */
/* i386_init */
static AsmArchPlugin * _i386_init(AsmArchPluginHelper * helper)
{
	AsmArchPlugin * plugin;

	if((plugin = object_new(sizeof(*plugin))) == NULL)
		return NULL;
	plugin->helper = helper;
	return plugin;
}


/* i386_destroy */
static void _i386_destroy(AsmArchPlugin * plugin)
{
	object_delete(plugin);
}


/* i386_decode */
static int _decode_constant(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i);
static int _decode_dregister(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i);
static int _decode_immediate(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i);
static int _decode_modrm(AsmArchPlugin * plugin, AsmArchInstructionCall * call,
		size_t * i);
static int _decode_modrm_do(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i, uint8_t u8);
static AsmArchInstruction const * _decode_opcode(AsmArchPlugin * plugin,
		AsmArchInstruction const * ai);
static int _decode_operand(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t * i);
static int _decode_postproc(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, unsigned int opcode);
static int _decode_register(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i);

static int _i386_decode(AsmArchPlugin * plugin, AsmArchInstructionCall * call)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchPrefix const * ap = NULL;
	AsmArchInstruction const * ai = NULL;
	unsigned int opcode;
	uint8_t u8;
	uint16_t u16;
	size_t i;

	/* FIXME detect end of input */
	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	opcode = u8;
	if((ap = helper->get_prefix_by_opcode(helper->arch, 8, opcode))
			!= NULL)
	{
		if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
			return -1;
		opcode = u8;
	}
	if((ai = helper->get_instruction_by_opcode(helper->arch, 8, opcode))
			== NULL)
	{
		u16 = u8 << 8;
		if(helper->peek(helper->arch, &u8, sizeof(u8)) == sizeof(u8))
		{
			u16 |= u8;
			if((ai = helper->get_instruction_by_opcode(helper->arch,
							16, u16)) != NULL)
			{
				opcode = u16;
				helper->read(helper->arch, &u8, sizeof(u8));
			}
		}
	}
	/* FIXME ignore prefixes when opcodes are not recognized */
	call->prefix = (ap != NULL) ? ap->name : NULL;
	if(ai == NULL)
	{
		/* no opcode was recognized */
		call->name = "db";
		call->operands[0].definition = AO_IMMEDIATE(0, 8, 0);
		call->operands[0].value.immediate.name = NULL;
		call->operands[0].value.immediate.value = opcode;
		call->operands[0].value.immediate.negative = 0;
		call->operands_cnt = 1;
		return 0;
	}
	if((ai = _decode_opcode(plugin, ai)) == NULL)
		return -1;
	call->name = ai->name;
	call->operands[0].definition = ai->op1;
	call->operands[1].definition = ai->op2;
	call->operands[2].definition = ai->op3;
	for(i = 0; i < 3 && AO_GET_TYPE(call->operands[i].definition)
			!= AOT_NONE; i++)
		if(_decode_operand(plugin, call, &i) != 0)
			return -1;
	call->operands_cnt = i;
	return _decode_postproc(plugin, call, opcode);
}

static int _decode_constant(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i)
{
	AsmArchOperandDefinition aod = call->operands[i].definition;
	AsmArchOperand * ao = &call->operands[i];
	(void) plugin;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(AO_GET_FLAGS(aod) & AOF_IMPLICIT)
	{
		ao->definition = AO_IMMEDIATE(0, AO_GET_SIZE(aod), 0);
		ao->value.immediate.name = NULL;
		ao->value.immediate.value = AO_GET_VALUE(aod);
		ao->value.immediate.negative = 0;
		return 0;
	}
	return -error_set_code(1, "%s", "Not implemented");
}

static int _decode_dregister(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperandDefinition aod = call->operands[i].definition;
	AsmArchRegister const * ar;
	uint8_t id;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* FIXME check the size */
	if(helper->read(helper->arch, &id, sizeof(id)) != sizeof(id))
		return -1;
	if((ar = helper->get_register_by_id_size(helper->arch, id,
					AO_GET_SIZE(aod))) == NULL)
		return -1;
	call->operands[i].value.dregister.name = ar->name;
	call->operands[i].value.dregister.offset = 0;
	return 0;
}

static int _decode_immediate(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperand * ao = &call->operands[i];
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() size=%u\n", __func__,
			AO_GET_SIZE(ao->definition) >> 3);
#endif
	ao->value.immediate.name = NULL;
	switch(AO_GET_SIZE(ao->definition) >> 3)
	{
		case sizeof(u8):
			if(helper->read(helper->arch, &u8, sizeof(u8))
					!= sizeof(u8))
				return -1;
			ao->value.immediate.value = u8;
			break;
		case sizeof(u16):
			if(helper->read(helper->arch, &u16, sizeof(u16))
					!= sizeof(u16))
				return -1;
			ao->value.immediate.value = _htol16(u16);
			break;
		case sizeof(u32):
			if(helper->read(helper->arch, &u32, sizeof(u32))
					!= sizeof(u32))
				return -1;
			ao->value.immediate.value = _htol32(u32);
			break;
		default:
			return -error_set_code(1, "%s", strerror(ENOSYS));
	}
	call->operands[i].value.immediate.negative = 0;
	return 0;
}

static int _decode_modrm(AsmArchPlugin * plugin, AsmArchInstructionCall * call,
		size_t * i)
{
	int ret = -1;
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperand * ao = call->operands;
	AsmArchOperand * ao1 = &call->operands[*i];
	AsmArchOperand * ao2 = NULL;
	uint8_t u8;
	uint8_t mod;
	uint8_t reg;
	uint8_t rm;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", &%lu)\n", __func__, call->name, *i);
#endif
	if(*i + 1 < 3 && (AO_GET_TYPE(ao[*i + 1].definition) == AOT_REGISTER
				|| AO_GET_TYPE(ao[*i + 1].definition)
				== AOT_DREGISTER))
		ao2 = &call->operands[*i + 1];
	/* FIXME invert ao1 and ao2 if the instruction is meant this way? */
	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	mod = (u8 >> 6) & 0x3;
	reg = (u8 >> 3) & 0x7;
	rm = u8 & 0x7;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: u8=0x%02x (%u %u %u)\n", u8, mod, reg, rm);
#endif
	if(AO_GET_TYPE(ao1->definition) == AOT_DREGISTER && ao2 != NULL
			&& AO_GET_TYPE(ao2->definition) & AOT_REGISTER
			&& AO_GET_FLAGS(ao2->definition) & AOF_I386_MODRM)
	{
		ret = _decode_modrm_do(plugin, call, (*i)++,
				(mod << 6) | (rm << 3));
		ret |= _decode_modrm_do(plugin, call, *i,
				(0x3 << 6) | (reg << 3));
	}
	else if(AO_GET_TYPE(ao1->definition) == AOT_REGISTER && ao2 != NULL
			&& AO_GET_FLAGS(ao2->definition) & AOF_I386_MODRM)
	{
		ret = _decode_modrm_do(plugin, call, (*i)++,
				(0x3 << 6) | (reg << 3));
		ret |= _decode_modrm_do(plugin, call, *i, (mod << 6) | rm);
	}
	else
		/* FIXME really implement */
		ret = _decode_modrm_do(plugin, call, *i, u8);
	return ret;
}

static int _decode_modrm_do(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i, uint8_t u8)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperand * ao = &call->operands[i];
	uint8_t mod;
	uint8_t reg;
	uint8_t rm;
	AsmArchRegister const * ar;
	uintW_t uW;

	mod = (u8 >> 6) & 0x3;
	reg = (u8 >> 3) & 0x7;
	rm = u8 & 0x7;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() u8=0x%02x (%u %u %u) size=%u\n", __func__,
			u8, mod, reg, rm, AO_GET_SIZE(ao->definition));
#endif
	if(mod == 3)
	{
		if((ar = helper->get_register_by_id_size(helper->arch, reg,
						AO_GET_SIZE(ao->definition)))
						== NULL)
			return -1;
		ao->definition = AO_REGISTER(0, 32, 0);
		ao->value._register.name = ar->name;
	}
	else if(mod == 2)
	{
		if((ar = helper->get_register_by_id_size(helper->arch, reg, W))
				== NULL)
			return -1;
		if(helper->read(helper->arch, &uW, sizeof(uW)) != sizeof(uW))
			return -1;
		ao->definition = AO_DREGISTER(0, W, W, 0);
		ao->value.dregister.name = ar->name;
		ao->value.dregister.offset = _htol32(uW); /* XXX _htolW() */
	}
	else if(mod == 1)
	{
		if((ar = helper->get_register_by_id_size(helper->arch, reg, W))
				== NULL)
			return -1;
		ao->definition = AO_DREGISTER(0, 8, W, 0);
		ao->value.dregister.name = ar->name;
	}
	else /* mod == 0 */
	{
		if(rm == 5) /* dispW */
		{
			/* FIXME SIB byte? */
			if(helper->read(helper->arch, &uW, sizeof(uW))
					!= sizeof(uW))
				return -1;
			/* FIXME endian */
			ao->definition = AO_IMMEDIATE(0, W, 0);
			ao->value.immediate.value = uW;
		}
		else if((ar = helper->get_register_by_id_size(helper->arch, reg,
						W)) != NULL)
		{
			ao->definition = AO_DREGISTER(0, 0, W, 0);
			ao->value.dregister.name = ar->name;
		}
		else
			return -1;
	}
	return 0;
}

static AsmArchInstruction const * _decode_opcode(AsmArchPlugin * plugin,
		AsmArchInstruction const * ai)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchInstruction const * p;
	size_t i;
	uint8_t mod;

	if(AO_GET_FLAGS(ai->op1) & AOF_I386_MODRM)
	{
		if(helper->peek(helper->arch, &mod, 1) != 1)
			return NULL;
		/* XXX this assumes helper->get_instruction_by_opcode() returns
		 * the first match, and from the plugin->instructions list */
		for(i = 0; arch_plugin.instructions[i].name != NULL
				&& &arch_plugin.instructions[i] != ai; i++);
		if(arch_plugin.instructions[i].name == NULL)
			return ai;
		for(i++; arch_plugin.instructions[i].name != NULL; i++)
		{
			p = &arch_plugin.instructions[i];
			if(p->opcode != ai->opcode || AO_GET_SIZE(ai->flags)
					!= AO_GET_SIZE(p->flags))
				continue;
			if((AO_GET_FLAGS(p->op1) & AOF_I386_MODRM)
					!= AOF_I386_MODRM)
				continue;
			if((mod & 0x07) == (AO_GET_VALUE(p->op1) & 0x07))
				return p;
		}
		/* XXX should we return NULL there? */
	}
	return ai;
}

static int _decode_operand(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t * i)
{
	AsmArchOperand * ao = &call->operands[*i];

	switch(AO_GET_TYPE(ao->definition))
	{
		case AOT_CONSTANT:
			return _decode_constant(plugin, call, *i);
		case AOT_DREGISTER:
			if(AO_GET_FLAGS(ao->definition) & AOF_I386_MODRM)
				return _decode_modrm(plugin, call, i);
			return _decode_dregister(plugin, call, *i);
		case AOT_IMMEDIATE:
			return _decode_immediate(plugin, call, *i);
		case AOT_REGISTER:
			if(AO_GET_FLAGS(ao->definition) & AOF_I386_MODRM)
				return _decode_modrm(plugin, call, i);
			return _decode_register(plugin, call, *i);
	}
	return -error_set_code(1, "%s", strerror(ENOSYS));
}

static int _decode_postproc(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, unsigned int opcode)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperand * ao;
	AsmFunction * af;

	switch(opcode)
	{
		case 0xe8: /* call */
			ao = &call->operands[0];
			ao->value.immediate.value += call->base + 5;
			af = helper->get_function_by_id(helper->arch,
					ao->value.immediate.value);
			if(af != NULL)
				ao->value.immediate.name = af->name;
			break;
		case 0xe9: /* jump */
			ao = &call->operands[0];
			ao->value.immediate.value += call->base + 5;
			break;
		case 0x0f80: /* jo */
		case 0x0f81: /* jno */
		case 0x0f82: /* jnae */
		case 0x0f83: /* jae, jnb, jnc */
		case 0x0f84: /* je, jz */
		case 0x0f85: /* jne */
		case 0x0f86: /* jna */
		case 0x0f87: /* ja, jnbe */
		case 0x0f88: /* js */
		case 0x0f89: /* jns */
		case 0x0f8a: /* jp, jpe */
		case 0x0f8b: /* jnp, jpo */
		case 0x0f8c: /* jl, jnge */
		case 0x0f8d: /* jnl, jge */
		case 0x0f8e: /* jle, jng */
		case 0x0f8f: /* jg, jnle */
			ao = &call->operands[0];
			ao->value.immediate.value += call->base + 6;
			break;
		case 0xeb: /* jump */
			ao = &call->operands[0];
			ao->value.immediate.value += call->base + 2;
			break;
	}
	return 0;
}

static int _decode_register(AsmArchPlugin * plugin,
		AsmArchInstructionCall * call, size_t i)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperandDefinition aod = call->operands[i].definition;
	AsmArchRegister const * ar;
	uint8_t id;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(AO_GET_FLAGS(aod) & AOF_IMPLICIT)
	{
		if((ar = helper->get_register_by_id_size(helper->arch,
						AO_GET_VALUE(aod),
						AO_GET_SIZE(aod))) == NULL)
			return -1;
		call->operands[i].value._register.name = ar->name;
		return 0;
	}
	/* FIXME check the size */
	if(helper->read(helper->arch, &id, sizeof(id)) != sizeof(id))
		return -1;
	if((ar = helper->get_register_by_id_size(helper->arch, id,
					AO_GET_SIZE(aod))) == NULL)
		return -1;
	call->operands[i].value._register.name = ar->name;
	return 0;
}


/* i386_encode */
static int _encode_constant(AsmArchPlugin * plugin,
		AsmArchOperandDefinition definition,
		AsmArchOperand const * operand);
static int _encode_dregister(AsmArchPlugin * plugin, uint32_t * i,
		AsmArchOperandDefinition * definitions,
		AsmArchOperand const * operands);
static int _encode_immediate(AsmArchPlugin * plugin,
		AsmArchOperand const * operand);
static int _encode_immediate8(AsmArchPlugin * plugin, uint8_t value);
static int _encode_immediate16(AsmArchPlugin * plugin, uint16_t value);
static int _encode_immediate24(AsmArchPlugin * plugin, uint32_t value);
static int _encode_immediate32(AsmArchPlugin * plugin, uint32_t value);
static int _encode_opcode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction);
static int _encode_operand(AsmArchPlugin * plugin, uint32_t * i,
		AsmArchOperandDefinition * definitions,
		AsmArchOperand const * operands);
static int _encode_prefix(AsmArchPlugin * plugin, AsmArchPrefix const * prefix);
static int _encode_register(AsmArchPlugin * plugin, uint32_t * i,
		AsmArchOperandDefinition * definitions,
		AsmArchOperand const * operands);

static int _i386_encode(AsmArchPlugin * plugin,
		AsmArchPrefix const * prefix,
		AsmArchInstruction const * instruction,
		AsmArchInstructionCall const * call)
{
	uint32_t i;
	AsmArchOperandDefinition definitions[3];

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	if(prefix != NULL && _encode_prefix(plugin, prefix) != 0)
		return -1;
	if(_encode_opcode(plugin, instruction) != 0)
		return -1;
	definitions[0] = instruction->op1;
	definitions[1] = instruction->op2;
	definitions[2] = instruction->op3;
	for(i = 0; i < call->operands_cnt; i++)
		if(_encode_operand(plugin, &i, definitions, call->operands)
				!= 0)
			return -1;
	return 0;
}

static int _encode_constant(AsmArchPlugin * plugin,
		AsmArchOperandDefinition definition,
		AsmArchOperand const * operand)
{
	AsmArchOperand ao;

	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT)
		return 0;
	ao = *operand;
	ao.definition &= ~(AOM_FLAGS);
	return _encode_immediate(plugin, &ao);
}

static int _encode_dregister(AsmArchPlugin * plugin, uint32_t * i,
		AsmArchOperandDefinition * definitions,
		AsmArchOperand const * operands)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperandDefinition definition = definitions[*i];
	AsmArchOperand const * operand = &operands[*i];
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);
	AsmArchRegister const * ar;
	AsmArchOperand ioperand;

	if((ar = helper->get_register_by_name_size(helper->arch, name, size))
			== NULL)
		return -1;
	/* write register */
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.definition = AO_IMMEDIATE(0, 8, 0);
	/* FIXME some combinations of register values are illegal */
	ioperand.value.immediate.value = ar->id;
	if(AO_GET_FLAGS(definition) & AOF_I386_MODRM
			&& AO_GET_VALUE(definition) == 8) /* mod r/m, /r */
	{
		(*i)++; /* skip next operand */
		/* FIXME it could as well be an inverted /r */
		name = operands[*i].value._register.name;
		size = AO_GET_SIZE(definitions[*i]);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		ioperand.value.immediate.value |= (ar->id << 3);
	}
	else if(AO_GET_FLAGS(definition) & AOF_I386_MODRM) /* mod r/m, /[0-7] */
		ioperand.value.immediate.value |= (AO_GET_VALUE(definition)
				<< 3);
	if(operand->value.dregister.offset == 0)
		/* there is no offset */
		return _encode_immediate(plugin, &ioperand);
	/* declare offset */
	switch(AO_GET_OFFSET(definition) >> 3)
	{
		case sizeof(uint8_t):
			ioperand.value.immediate.value |= 0x40;
			break;
		case W >> 3:
			ioperand.value.immediate.value |= 0x80;
			break;
		default:
			return -error_set_code(1, "%s", "Invalid offset");
	}
	if(_encode_immediate(plugin, &ioperand) != 0)
		return -1;
	/* write offset */
	ioperand.definition = AO_IMMEDIATE(0, AO_GET_OFFSET(definition), 0);
	ioperand.value.immediate.value = operand->value.dregister.offset;
	return _encode_immediate(plugin, &ioperand);
}

static int _encode_immediate(AsmArchPlugin * plugin,
		AsmArchOperand const * operand)
{
	uint64_t value = operand->value.immediate.value;

	if((AO_GET_FLAGS(operand->definition) & AOF_SIGNED)
			&& operand->value.immediate.negative != 0)
		value = -value; /* XXX check */
	switch(AO_GET_SIZE(operand->definition) >> 3)
	{
		case 0:
			return 0;
		case sizeof(uint8_t):
			return _encode_immediate8(plugin, value);
		case sizeof(uint16_t):
			return _encode_immediate16(plugin, value);
		case 3:
			return _encode_immediate24(plugin, value);
		case sizeof(uint32_t):
			return _encode_immediate32(plugin, value);
	}
	return -error_set_code(1, "%s", "Invalid size");
}

static int _encode_immediate8(AsmArchPlugin * plugin, uint8_t value)
{
	AsmArchPluginHelper * helper = plugin->helper;

	if(helper->write(helper->arch, &value, sizeof(value)) != sizeof(value))
		return -1;
	return 0;
}

static int _encode_immediate16(AsmArchPlugin * plugin, uint16_t value)
{
	AsmArchPluginHelper * helper = plugin->helper;

	value = _htol16(value);
	if(helper->write(helper->arch, &value, sizeof(value)) != sizeof(value))
		return -1;
	return 0;
}

static int _encode_immediate24(AsmArchPlugin * plugin, uint32_t value)
{
	AsmArchPluginHelper * helper = plugin->helper;

	value = _htol32(value) >> 8;
	if(helper->write(helper->arch, &value, 3) != 3)
		return -1;
	return 0;
}

static int _encode_immediate32(AsmArchPlugin * plugin, uint32_t value)
{
	AsmArchPluginHelper * helper = plugin->helper;

	value = _htol32(value);
	if(helper->write(helper->arch, &value, sizeof(value)) != sizeof(value))
		return -1;
	return 0;
}

static int _encode_opcode(AsmArchPlugin * plugin,
		AsmArchInstruction const * instruction)
{
	AsmArchOperand operand;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() size=%u opcode=0x%x\n", __func__,
			AO_GET_SIZE(instruction->flags), instruction->opcode);
#endif
	memset(&operand, 0, sizeof(operand));
	operand.definition = AO_IMMEDIATE(0, AO_GET_SIZE(instruction->flags),
			0);
	switch(AO_GET_SIZE(instruction->flags) >> 3)
	{
		case 0:
			return 0;
		case sizeof(uint8_t):
			operand.value.immediate.value = instruction->opcode;
			break;
		case sizeof(uint16_t):
			operand.value.immediate.value = _htob16(
					instruction->opcode);
			break;
		case 3:
		case sizeof(uint32_t):
			operand.value.immediate.value = _htob32(
					instruction->opcode);
			break;
		default:
			return -error_set_code(1, "%s", "Invalid size");
	}
	return _encode_immediate(plugin, &operand);
}

static int _encode_operand(AsmArchPlugin * plugin, uint32_t * i,
		AsmArchOperandDefinition * definitions,
		AsmArchOperand const * operands)
{
	switch(operands[*i].definition)
	{
		case AOT_CONSTANT:
			return _encode_constant(plugin, definitions[*i],
					&operands[*i]);
		case AOT_DREGISTER:
			return _encode_dregister(plugin, i, definitions,
					operands);
		case AOT_IMMEDIATE:
			return _encode_immediate(plugin, &operands[*i]);
		case AOT_REGISTER:
			return _encode_register(plugin, i, definitions,
					operands);
		case AOT_NONE:
		case AOT_DREGISTER2:
			/* should not happen */
			break;
	}
	return 0;
}

static int _encode_prefix(AsmArchPlugin * plugin, AsmArchPrefix const * prefix)
{
	AsmArchOperand operand;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() size=%u opcode=0x%x\n", __func__,
			AO_GET_SIZE(prefix->flags), prefix->opcode);
#endif
	memset(&operand, 0, sizeof(operand));
	operand.definition = AO_IMMEDIATE(0, AO_GET_SIZE(prefix->flags), 0);
	switch(AO_GET_SIZE(prefix->flags) >> 3)
	{
		case 0:
			return 0;
		case sizeof(uint8_t):
			operand.value.immediate.value = prefix->opcode;
			break;
		case sizeof(uint16_t):
			operand.value.immediate.value = _htob16(prefix->opcode);
			break;
		case 3:
		case sizeof(uint32_t):
			operand.value.immediate.value = _htob32(prefix->opcode);
			break;
		default:
			return -error_set_code(1, "%s", "Invalid size");
	}
	return _encode_immediate(plugin, &operand);
}

static int _encode_register(AsmArchPlugin * plugin, uint32_t * i,
		AsmArchOperandDefinition * definitions,
		AsmArchOperand const * operands)
{
	AsmArchPluginHelper * helper = plugin->helper;
	AsmArchOperandDefinition definition = definitions[*i];
	AsmArchOperand const * operand = &operands[*i];
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);
	AsmArchRegister const * ar;
	AsmArchOperand ioperand;

	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT)
		return 0;
	if((ar = helper->get_register_by_name_size(helper->arch, name, size))
			== NULL)
		return -1;
	/* write register */
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.definition = AO_IMMEDIATE(0, 8, 0);
	ioperand.value.immediate.value = ar->id;
	if(AO_GET_FLAGS(definition) & AOF_I386_MODRM
			&& AO_GET_VALUE(definition) == 8) /* mod r/m, /r */
	{
		(*i)++; /* skip next operand */
		/* FIXME it could as well be an inverted /r */
		name = operands[*i].value._register.name;
		size = AO_GET_SIZE(definitions[*i]);
		if((ar = helper->get_register_by_name_size(helper->arch, name,
						size)) == NULL)
			return -1;
		ioperand.value.immediate.value |= 0xc0 | (ar->id << 3);
	}
	else if(AO_GET_FLAGS(definition) & AOF_I386_MODRM) /* mod r/m, /[0-7] */
		ioperand.value.immediate.value = 0xc0 | ar->id
			| (AO_GET_VALUE(definition) << 3);
	else
		ioperand.value.immediate.value = ar->id;
	return _encode_immediate(plugin, &ioperand);
}

#endif /* !ASM_ARCH_I386_H */
