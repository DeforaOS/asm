/* $Id$ */
/* Copyright (c) 2011 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel asm */
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



#include <System.h>
#include <string.h>
#include <errno.h>


/* i386 */
/* private */
/* prototypes */
static int _i386_decode(ArchPlugin * plugin, ArchInstructionCall * call);
static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call);


/* functions */
/* i386_decode */
static int _decode_constant(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);
static int _decode_dregister(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);
static int _decode_immediate(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);
static int _decode_modrm(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t * i);
static int _decode_modrm_do(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i, uint8_t u8);
static int _decode_operand(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t * i);
static int _decode_register(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i);

static int _i386_decode(ArchPlugin * plugin, ArchInstructionCall * call)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchInstruction * ai = NULL;
	uint8_t u8;
	uint16_t u16;
	size_t i;

	/* FIXME detect end of input */
	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	call->operands[0].type = AOT_NONE;
	call->operands[1].type = AOT_NONE;
	call->operands[2].type = AOT_NONE;
	if((ai = helper->get_instruction_by_opcode(helper->arch, 8, u8))
			== NULL)
	{
		u16 = u8;
		if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		{
			call->name = "db";
			call->operands[0].type = AO_IMMEDIATE(0, 8, 0);
			call->operands[0].value.immediate.value = u8;
			call->operands[0].value.immediate.negative = 0;
			call->operands_cnt = 1;
			return 0;
		}
		u16 = (u16 << 8) | u8;
		if((ai = helper->get_instruction_by_opcode(helper->arch, 16,
						u16)) == NULL)
		{
			call->name = "dw";
			call->operands[0].type = AO_IMMEDIATE(0, 16, 0);
			call->operands[0].value.immediate.value = u16;
			call->operands[0].value.immediate.negative = 0;
			call->operands_cnt = 1;
			return 0;
		}
	}
	call->name = ai->name;
	call->operands[0].type = ai->op1;
	call->operands[1].type = ai->op2;
	call->operands[2].type = ai->op3;
	for(i = 0; i < 3 && AO_GET_TYPE(call->operands[i].type) != 0; i++)
		if(_decode_operand(plugin, call, &i) != 0)
			return -1;
	call->operands_cnt = i;
	return 0;
}

static int _decode_constant(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	ArchOperandDefinition aod = call->operands[i].type;
	ArchOperand * ao = &call->operands[i];

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(AO_GET_FLAGS(aod) & AOF_IMPLICIT)
	{
		ao->type = AO_IMMEDIATE(0, AO_GET_SIZE(aod), 0);
		ao->value.immediate.value = AO_GET_VALUE(aod);
		return 0;
	}
	return -error_set_code(1, "%s", "Not implemented");
}

static int _decode_dregister(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition aod = call->operands[i].type;
	ArchRegister * ar;
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

static int _decode_immediate(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperand * ao = &call->operands[i];
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() size=%u\n", __func__,
			AO_GET_SIZE(ao->type) >> 3);
#endif
	switch(AO_GET_SIZE(ao->type) >> 3)
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

static int _decode_modrm(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t * i)
{
	int ret = -1;
	ArchPluginHelper * helper = plugin->helper;
	ArchOperand * ao1 = &call->operands[*i];
	ArchOperand * ao2 = NULL;
	uint8_t u8;
	uint8_t mod;
	uint8_t reg;
	uint8_t rm;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", &%lu)\n", __func__, call->name, *i);
#endif
	if(*i + 1 < 3 && (AO_GET_TYPE(call->operands[*i + 1].type)
				== AOT_REGISTER
				|| AO_GET_TYPE(call->operands[*i + 1].type)
				== AOT_DREGISTER))
		ao2 = &call->operands[*i + 1];
	if(helper->read(helper->arch, &u8, sizeof(u8)) != sizeof(u8))
		return -1;
	mod = (u8 >> 6) & 0x3;
	reg = (u8 >> 3) & 0x7;
	rm = u8 & 0x7;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: u8=0x%02x (%u %u %u)\n", u8, mod, reg, rm);
#endif
	if(AO_GET_TYPE(ao1->type) == AOT_DREGISTER && ao2 != NULL
			&& AO_GET_TYPE(ao2->type) & AOT_REGISTER
			&& AO_GET_FLAGS(ao2->type) & AOF_I386_MODRM)
	{
		ret = _decode_modrm_do(plugin, call, (*i)++,
				(mod << 6) | (rm << 3));
		ret |= _decode_modrm_do(plugin, call, *i,
				(0x3 << 6) | (reg << 3));
	}
	else if(AO_GET_TYPE(ao1->type) == AOT_REGISTER && ao2 != NULL
			&& AO_GET_FLAGS(ao2->type) & AOF_I386_MODRM)
	{
		ret = _decode_modrm_do(plugin, call, (*i)++,
				(0x3 << 6) | (reg << 3));
		ret |= _decode_modrm_do(plugin, call, *i,
				(mod << 6) | (rm << 3));
	}
	else
		/* FIXME really implement */
		ret = _decode_modrm_do(plugin, call, *i, u8);
	return ret;
}

static int _decode_modrm_do(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i, uint8_t u8)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperand * ao = &call->operands[i];
	uint8_t mod;
	uint8_t reg;
	uint8_t rm;
	ArchRegister * ar;
	uintW_t uW;

	mod = (u8 >> 6) & 0x3;
	reg = (u8 >> 3) & 0x7;
	rm = u8 & 0x7;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: u8=0x%02x (%u %u %u) size=%u\n",
			u8, mod, reg, rm, AO_GET_SIZE(ao->type));
#endif
	if(mod == 3)
	{
		if((ar = helper->get_register_by_id_size(helper->arch, reg,
						AO_GET_SIZE(ao->type))) == NULL)
			return -1;
		ao->type = AO_REGISTER(0, 32, 0);
		ao->value._register.name = ar->name;
	}
	else if(mod == 2)
	{
		if((ar = helper->get_register_by_id_size(helper->arch, reg, W))
				== NULL)
			return -1;
		if(helper->read(helper->arch, &uW, sizeof(uW)) != sizeof(uW))
			return -1;
		ao->type = AO_DREGISTER(0, W, W, 0);
		ao->value.dregister.name = ar->name;
		ao->value.dregister.offset = _htol32(uW); /* XXX _htolW() */
	}
	else if(mod == 1)
	{
		if((ar = helper->get_register_by_id_size(helper->arch, reg, W))
				== NULL)
			return -1;
		ao->type = AO_DREGISTER(0, 8, W, 0);
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
			ao->type = AO_IMMEDIATE(0, W, 0);
			ao->value.immediate.value = uW;
		}
		else if((ar = helper->get_register_by_id_size(helper->arch, reg,
						W)) != NULL)
		{
			ao->type = AO_DREGISTER(0, 0, W, 0);
			ao->value.dregister.name = ar->name;
		}
		else
			return -1;
	}
	return 0;
}

static int _decode_operand(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t * i)
{
	ArchOperand * ao = &call->operands[*i];

	switch(AO_GET_TYPE(ao->type))
	{
		case AOT_CONSTANT:
			return _decode_constant(plugin, call, *i);
		case AOT_DREGISTER:
			if(AO_GET_FLAGS(ao->type) & AOF_I386_MODRM)
				return _decode_modrm(plugin, call, i);
			return _decode_dregister(plugin, call, *i);
		case AOT_IMMEDIATE:
			return _decode_immediate(plugin, call, *i);
		case AOT_REGISTER:
			if(AO_GET_FLAGS(ao->type) & AOF_I386_MODRM)
				return _decode_modrm(plugin, call, i);
			return _decode_register(plugin, call, *i);
	}
	return -error_set_code(1, "%s", strerror(ENOSYS));
}

static int _decode_register(ArchPlugin * plugin, ArchInstructionCall * call,
		size_t i)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition aod = call->operands[i].type;
	ArchRegister * ar;
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


/* i386_write */
static int _write_constant(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand);
static int _write_dregister(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands);
static int _write_immediate(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand);
static int _write_immediate8(ArchPlugin * plugin, uint8_t value);
static int _write_immediate16(ArchPlugin * plugin, uint16_t value);
static int _write_immediate24(ArchPlugin * plugin, uint32_t value);
static int _write_immediate32(ArchPlugin * plugin, uint32_t value);
static int _write_opcode(ArchPlugin * plugin, ArchInstruction * instruction);
static int _write_operand(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands);
static int _write_register(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands);

static int _i386_write(ArchPlugin * plugin, ArchInstruction * instruction,
		ArchInstructionCall * call)
{
	uint32_t i;
	ArchOperandDefinition definitions[3];

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, instruction->name);
#endif
	if(_write_opcode(plugin, instruction) != 0)
		return -1;
	definitions[0] = instruction->op1;
	definitions[1] = instruction->op2;
	definitions[2] = instruction->op3;
	for(i = 0; i < call->operands_cnt; i++)
		if(_write_operand(plugin, &i, definitions, call->operands) != 0)
			return -1;
	return 0;
}

static int _write_constant(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT)
		return 0;
	definition &= ~(AOM_FLAGS);
	return _write_immediate(plugin, definition, operand);
}

static int _write_dregister(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition definition = definitions[*i];
	ArchOperand * operand = &operands[*i];
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);
	ArchRegister * ar;
	ArchOperandDefinition idefinition;
	ArchOperand ioperand;

	if((ar = helper->get_register_by_name_size(helper->arch, name, size))
			== NULL)
		return -1;
	/* write register */
	idefinition = AO_IMMEDIATE(0, 8, 0);
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.type = AOT_IMMEDIATE;
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
		return _write_immediate(plugin, idefinition, &ioperand);
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
	if(_write_immediate(plugin, idefinition, &ioperand) != 0)
		return -1;
	/* write offset */
	idefinition = AO_IMMEDIATE(0, AO_GET_OFFSET(definition), 0);
	ioperand.value.immediate.value = operand->value.dregister.offset;
	return _write_immediate(plugin, idefinition, &ioperand);
}

static int _write_immediate(ArchPlugin * plugin,
		ArchOperandDefinition definition, ArchOperand * operand)
{
	uint64_t value = operand->value.immediate.value;

	if((AO_GET_FLAGS(definition) & AOF_SIGNED)
			&& operand->value.immediate.negative != 0)
		value = -value;
	switch(AO_GET_SIZE(definition) >> 3)
	{
		case 0:
			return 0;
		case sizeof(uint8_t):
			return _write_immediate8(plugin, value);
		case sizeof(uint16_t):
			return _write_immediate16(plugin, value);
		case 3:
			return _write_immediate24(plugin, value);
		case sizeof(uint32_t):
			return _write_immediate32(plugin, value);
	}
	return -error_set_code(1, "Invalid size");
}

static int _write_immediate8(ArchPlugin * plugin, uint8_t value)
{
	ArchPluginHelper * helper = plugin->helper;

	if(helper->write(helper->arch, &value, sizeof(value)) != sizeof(value))
		return -1;
	return 0;
}

static int _write_immediate16(ArchPlugin * plugin, uint16_t value)
{
	ArchPluginHelper * helper = plugin->helper;

	value = _htol16(value);
	if(helper->write(helper->arch, &value, sizeof(value)) != sizeof(value))
		return -1;
	return 0;
}

static int _write_immediate24(ArchPlugin * plugin, uint32_t value)
{
	ArchPluginHelper * helper = plugin->helper;

	value = _htol32(value) >> 8;
	if(helper->write(helper->arch, &value, 3) != 3)
		return -1;
	return 0;
}

static int _write_immediate32(ArchPlugin * plugin, uint32_t value)
{
	ArchPluginHelper * helper = plugin->helper;

	value = _htol32(value);
	if(helper->write(helper->arch, &value, sizeof(value)) != sizeof(value))
		return -1;
	return 0;
}

static int _write_opcode(ArchPlugin * plugin, ArchInstruction * instruction)
{
	ArchOperand operand;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() size=%u opcode=0x%x\n", __func__,
			AO_GET_SIZE(instruction->flags), instruction->opcode);
#endif
	memset(&operand, 0, sizeof(operand));
	operand.type = AOT_IMMEDIATE;
	switch(AO_GET_SIZE(instruction->flags) >> 3)
	{
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
	return _write_immediate(plugin, instruction->flags, &operand);
}

static int _write_operand(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands)
{
	switch(operands[*i].type)
	{
		case AOT_CONSTANT:
			return _write_constant(plugin, definitions[*i],
					&operands[*i]);
		case AOT_DREGISTER:
			return _write_dregister(plugin, i, definitions,
					operands);
		case AOT_IMMEDIATE:
			return _write_immediate(plugin, definitions[*i],
					&operands[*i]);
		case AOT_REGISTER:
			return _write_register(plugin, i, definitions,
					operands);
		case AOT_NONE:
		case AOT_DREGISTER2:
			/* should not happen */
			break;
	}
	return 0;
}

static int _write_register(ArchPlugin * plugin, uint32_t * i,
		ArchOperandDefinition * definitions, ArchOperand * operands)
{
	ArchPluginHelper * helper = plugin->helper;
	ArchOperandDefinition definition = definitions[*i];
	ArchOperand * operand = &operands[*i];
	char const * name = operand->value._register.name;
	size_t size = AO_GET_SIZE(definition);
	ArchRegister * ar;
	ArchOperandDefinition idefinition;
	ArchOperand ioperand;

	if(AO_GET_FLAGS(definition) & AOF_IMPLICIT)
		return 0;
	if((ar = helper->get_register_by_name_size(helper->arch, name, size))
			== NULL)
		return -1;
	/* write register */
	idefinition = AO_IMMEDIATE(0, 8, 0);
	memset(&ioperand, 0, sizeof(ioperand));
	ioperand.type = AOT_IMMEDIATE;
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
	return _write_immediate(plugin, idefinition, &ioperand);
}
