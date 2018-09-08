/* $Id$ */
/* Copyright (c) 2011-2018 Pierre Pronchery <khorben@defora.org> */
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
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "common.h"
#include "token.h"
#include "parser.h"

#ifndef PROGNAME
# define PROGNAME "asm"
#endif


/* private */
/* types */
typedef struct _State
{
	Cpp * cpp;
	Token * token;
	unsigned int error_cnt;
	unsigned int warning_cnt;
	AsmCode * code;

	/* directive */
	char * directive;
	char ** args;
	size_t args_cnt;

	/* instruction */
	AsmArchInstructionCall call;

	/* operands */
	int address;
	int negative;
} State;


/* prototypes */
static int _parser_check(State * state, TokenCode code);
static int _parser_defines(State * state, AsmPrefs * ap);
static int _parser_error(State * state, char const * format, ...);
static int _parser_is_code(State * state, TokenCode code);
static int _parser_in_set(State * state, TokenSet set);
static int _parser_recover(State * state, TokenCode code, char const * name);
static int _parser_scan(State * state);
static int _parser_warning(State * state, char const * format, ...);

/* grammar */
static int _program(State * state);
static int _directive(State * state);
static int _directive_arg(State * state);
static int _directive_args(State * state);
static int _directive_name(State * state);
static int _section_args(State * state);
static int _section_name(State * state);
static int _newline(State * state);
static int _space(State * state);
static int _statement(State * state);
static int _function(State * state);
static int _function_name(State * state);
static int _instruction(State * state);
static int _instruction_name(State * state);
static int _operand_list(State * state);
static int _operand(State * state);
static int _prefix(State * state);
static int _value(State * state);
static int _symbol(State * state);
static int _register(State * state);
static int _immediate(State * state);
static int _address(State * state);
static int _sign(State * state);
static int _offset(State * state);


/* functions */
/* parser_recover */
static int _parser_recover(State * state, TokenCode code, char const * name)
{
	while(!_parser_is_code(state, code))
		_parser_scan(state);
	return _parser_error(state, "%s%s", name, " expected");
}


/* parser_scan */
static int _scan_skip_meta(State * state);

static int _parser_scan(State * state)
{
	int ret;
	TokenCode code;
	char const * string;

	if(state->token != NULL)
		token_delete(state->token);
	if((ret = _scan_skip_meta(state)) != 0
			|| state->token == NULL)
		return ret;
	code = token_get_code(state->token);
	string = token_get_string(state->token);
	if(code == AS_CODE_WORD)
	{
		if(string != NULL && string[0] == '$')
			token_set_code(state->token, AS_CODE_IMMEDIATE);
	}
	else if(code == AS_CODE_OPERATOR_MODULO)
	{
		/* FIXME ugly workaround */
		if((ret = _scan_skip_meta(state)) != 0)
			return ret;
		if(_parser_is_code(state, AS_CODE_WORD))
			token_set_code(state->token, AS_CODE_REGISTER);
	}
	return 0;
}

static int _scan_skip_meta(State * state)
{
	int ret = 0;
	TokenCode code;

	while(cpp_scan(state->cpp, &state->token) == 0)
	{
		if(state->token == NULL)
			return ret;
		if((code = token_get_code(state->token)) < AS_CODE_META_FIRST
				|| code > AS_CODE_META_LAST)
			return ret;
		if(code == AS_CODE_META_ERROR)
			ret |= _parser_error(state, "%s", token_get_string(
						state->token));
		else if(code == AS_CODE_META_WARNING)
			_parser_warning(state, "%s", token_get_string(
						state->token));
		token_delete(state->token);
	}
	return 1;
}


/* parser_check */
static int _parser_check(State * state, TokenCode code)
{
	int ret = 0;

	if(!_parser_is_code(state, code))
		/* FIXME convert the code to a string */
		ret = _parser_error(state, "%s%u", "Parse error: expected ",
				code);
	ret |= _parser_scan(state);
	return ret;
}


/* parser_defines */
static int _parser_defines(State * state, AsmPrefs * ap)
{
	int ret = 0;
	char const * p;
	char * q;
	size_t len;
	size_t i;

	if((p = asmcode_get_arch(state->code)) != NULL
			&& (len = strlen(p)) > 0)
	{
		if((q = malloc(len + 5)) == NULL)
			return -error_set_code(1, "%s", strerror(errno));
		snprintf(q, len + 5, "__%s__", p);
		ret |= cpp_define_add(state->cpp, q, NULL);
		free(q);
	}
	if(ret == 0 && ap != NULL)
		for(i = 0; i < ap->defines_cnt; i++)
			ret |= cpp_define_add(state->cpp, ap->defines[i].name,
					ap->defines[i].value);
	return ret;
}


/* parser_is_code */
static int _parser_is_code(State * state, TokenCode code)
{
	if(state->token == NULL)
		return 0;
	return token_get_code(state->token) == code;
}


/* parser_in_set */
static int _parser_in_set(State * state, TokenSet set)
{
	if(state->token == NULL)
		return 0;
	return token_in_set(state->token, set);
}


/* parser_error */
static int _parser_error(State * state, char const * format, ...)
{
	va_list ap;

	fputs(PROGNAME ": ", stderr);
	if(state->cpp != NULL && state->token != NULL)
		/* FIXME will be wrong when string-based input is supported */
		fprintf(stderr, "%s%s%u: ", cpp_get_filename(state->cpp),
				", line ", token_get_line(state->token));
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
	return ++state->error_cnt;
}


/* parser_warning */
static int _parser_warning(State * state, char const * format, ...)
{
	va_list ap;

	fputs(PROGNAME ": ", stderr);
	if(state->cpp != NULL && state->token != NULL)
		/* FIXME will be wrong when string-based input is supported */
		fprintf(stderr, "%s%s%u: ", cpp_get_filename(state->cpp),
				", line ", token_get_line(state->token));
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
	return ++state->warning_cnt;
}


/* protected */
/* functions */
/* parser */
int parser(AsmPrefs * ap, AsmCode * code, char const * infile)
{
	CppPrefs prefs;
	State state;

	memset(&prefs, 0, sizeof(prefs));
	prefs.filename = infile;
	prefs.filters = CPP_FILTER_COMMENT;
	memset(&state, 0, sizeof(state));
	state.code = code;
	if((state.cpp = cpp_new(&prefs)) == NULL)
		return -1;
	if(_parser_defines(&state, ap) != 0)
	{
		cpp_delete(state.cpp);
		return -1;
	}
	if(_parser_scan(&state) != 0)
	{
		cpp_delete(state.cpp);
		return _parser_error(&state, "%s", error_get(NULL));
	}
	if(_program(&state) != 0)
		error_set_code(1, "%s%s%u%s%u%s", infile,
				": Compilation failed with ", state.error_cnt,
				" error(s) and ", state.warning_cnt,
				" warning(s)");
	if(state.token != NULL)
		token_delete(state.token);
	return state.error_cnt;
}


/* parser_string */
int parser_string(AsmPrefs * ap, AsmCode * code, char const * string)
{
	CppPrefs prefs;
	State state;
	size_t i;

	memset(&prefs, 0, sizeof(prefs));
#if 0
	prefs.filename = infile;
#endif
	prefs.filters = CPP_FILTER_COMMENT;
	memset(&state, 0, sizeof(state));
	state.code = code;
	if((state.cpp = cpp_new_string(&prefs, string)) == NULL)
		return -1;
	if(ap != NULL)
		for(i = 0; i < ap->defines_cnt; i++)
			if(cpp_define_add(state.cpp, ap->defines[i].name,
						ap->defines[i].value) != 0)
			{
				cpp_delete(state.cpp);
				return -1;
			}
	if(_parser_scan(&state) != 0)
		return _parser_error(&state, "%s", error_get(NULL));
	if(_program(&state) != 0)
		error_set_code(1, "%s%u%s%u%s", "Compilation failed with ",
				state.error_cnt, " error(s) and ",
				state.warning_cnt, " warning(s)");
	if(state.token != NULL)
		token_delete(state.token);
	return state.error_cnt;
}


/* grammar */
/* program */
static int _program(State * state)
	/* { directive | statement } */
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	for(;;)
	{
		if(_parser_in_set(state, TS_DIRECTIVE))
			/* directive */
			ret |= _directive(state);
		else if(_parser_in_set(state, TS_STATEMENT))
			/* statement */
			ret |= _statement(state);
		else
			/* end of input (FIXME really check) */
			break;
	}
	return ret;
}


/* directive */
static int _directive(State * state)
	/* "." directive_name [ space [ directive_args ] ] newline */
{
	int ret;
	size_t i;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* "." */
	ret = _parser_check(state, AS_CODE_OPERATOR_DOT);
	/* directive_name */
	if(!_parser_in_set(state, TS_DIRECTIVE_NAME))
		return _parser_recover(state, AS_CODE_NEWLINE,
				"Directive name");
	ret |= _directive_name(state);
	/* [ space [ directive_args ] ] */
	if(_parser_in_set(state, TS_SPACE))
	{
		ret |= _space(state);
		if(_parser_in_set(state, TS_DIRECTIVE_ARGS))
		{
			/* XXX hack to allow dots in section names */
			if(string_compare(state->directive, "section") == 0)
				ret |= _section_args(state);
			else
				ret |= _directive_args(state);
		}
	}
	/* execute the directive */
	if(string_compare(state->directive, "section") == 0)
	{
		if(state->args_cnt != 1)
			ret |= _parser_error(state, "%s",
					"Sections expect a name");
		else if(asmcode_section(state->code, state->args[0]) != 0)
			ret |= _parser_error(state, "%s", error_get(NULL));
	}
	else if(asmcode_directive(state->code, state->directive,
				(char const **)state->args,
				state->args_cnt) != 0)
		ret |= _parser_error(state, "%s", error_get(NULL));
	free(state->directive);
	state->directive = NULL;
	for(i = 0; i < state->args_cnt; i++)
		free(state->args[i]);
	free(state->args);
	state->args = NULL;
	state->args_cnt = 0;
	/* newline */
	if(!_parser_in_set(state, TS_NEWLINE))
		return _parser_recover(state, AS_CODE_NEWLINE, "New line");
	ret |= _newline(state);
	return ret;
}


/* directive_arg */
static int _directive_arg(State * state)
	/* WORD | NUMBER | "." */
{
	char const * string;
	char ** p;

	if(state->token == NULL
			|| (string = token_get_string(state->token)) == NULL
			|| strlen(token_get_string(state->token)) == 0)
		return error_set_code(1, "%s",
				"Empty directive arguments are not allowed");
	if((p = realloc(state->args, sizeof(*p) * (state->args_cnt + 1)))
			== NULL)
		return error_set_code(1, "%s", strerror(errno));
	state->args = p;
	p = &state->args[state->args_cnt];
	if((*p = strdup(string)) == NULL)
		return error_set_code(1, "%s", strerror(errno));
	state->args_cnt++;
	return _parser_scan(state);
}


/* directive_args */
static int _directive_args(State * state)
	/* directive_arg { space [ directive_arg ] } */
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	ret = _directive_arg(state);
	while(_parser_in_set(state, TS_SPACE))
	{
		/* space */
		ret |= _space(state);
		/* [ directive_arg ] */
		if(_parser_in_set(state, TS_DIRECTIVE_ARGS))
			ret |= _directive_arg(state);
	}
	return ret;
}


/* directive_name */
static int _directive_name(State * state)
	/* WORD */
{
	int ret = 0;
	char const * string;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(state->token == NULL
			|| (string = token_get_string(state->token)) == NULL
			|| strlen(token_get_string(state->token)) == 0)
		return error_set_code(1, "%s",
				"Directives with empty names are not allowed");
	if((state->directive = strdup(string)) == NULL)
		return ret | error_set_code(1, "%s", strerror(errno));
	ret |= _parser_scan(state);
#ifdef DEBUG
	fprintf(stderr, "%s\"%s\"\n", "DEBUG: directive ", state->directive);
#endif
	return ret;
}


/* section_args */
static int _section_args(State * state)
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	ret = _section_name(state);
	while(_parser_in_set(state, TS_DIRECTIVE_ARGS))
		/* section_name */
		ret |= _section_name(state);
	return ret;
}


/* section_name */
static int _section_name(State * state)
	/* WORD | NUMBER | "." */
{
	char const * string;
	char * p;
	size_t len;

	if(state->token == NULL
			|| (string = token_get_string(state->token)) == NULL
			|| (len = strlen(token_get_string(state->token))) == 0)
		return error_set_code(1, "%s",
				"Empty section arguments are not allowed");
	if(state->args == NULL)
	{
		if((state->args = malloc(sizeof(string))) == NULL
				|| (state->args[0] = strdup(string)) == NULL)
			return error_set_code(1, "%s", strerror(errno));
		state->args_cnt = 1;
		return _parser_scan(state);
	}
	if((p = realloc(state->args[0], strlen(state->args[0]) + len + 1))
			== NULL)
		return error_set_code(1, "%s", strerror(errno));
	state->args[0] = p;
	strcat(state->args[0], string);
	return _parser_scan(state);
}


/* newline */
static int _newline(State * state)
	/* [ space ] NEWLINE */
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* [ space ] */
	if(_parser_in_set(state, TS_SPACE))
		ret |= _space(state);
	/* NEWLINE */
	ret |= _parser_check(state, AS_CODE_NEWLINE);
	return ret;
}


/* space */
static int _space(State * state)
	/* SPACE { SPACE } */
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	ret = _parser_check(state, AS_CODE_WHITESPACE);
	while(_parser_is_code(state, AS_CODE_WHITESPACE))
		ret |= _parser_scan(state);
	return ret;
}


/* statement */
static int _statement(State * state)
	/* ( function | [ space [ instruction ] ] ) newline */
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(_parser_in_set(state, TS_FUNCTION))
		/* function */
		ret = _function(state);
	else if(_parser_in_set(state, TS_SPACE))
	{
		/* space [ instruction ] */
		ret = _space(state);
		if(_parser_in_set(state, TS_INSTRUCTION))
			ret |= _instruction(state);
	}
	else if(!_parser_in_set(state, TS_NEWLINE))
		return _parser_recover(state, AS_CODE_NEWLINE, "Statement");
	ret |= _newline(state);
	return ret;
}


/* function */
static int _function(State * state)
	/* function_name ":" */
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	ret = _function_name(state);
	ret |= _parser_check(state, AS_CODE_OPERATOR_COLON);
	return ret;
}


/* function_name */
static int _function_name(State * state)
	/* WORD */
{
	int ret;
	char const * string;
	char * function = NULL;

	if((string = token_get_string(state->token)) == NULL)
		return _parser_error(state, "%s", "Empty function names are"
				" not allowed");
	if((function = strdup(string)) == NULL)
		return error_set_code(1, "%s", strerror(errno));
	ret = _parser_check(state, AS_CODE_WORD);
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s \"%s\"\n", "function", function);
#endif
	if(asmcode_function(state->code, function) != 0)
		ret |= _parser_error(state, "%s", error_get(NULL));
	free(function);
	return ret;
}


/* instruction */
static int _instruction(State * state)
	/* [ prefix space ] instruction_name [ space [ operand_list ] ] */
{
	int ret = 0;
	char const * string;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* reset the current instruction */
	memset(&state->call, 0, sizeof(state->call));
	/* [ prefix space ] */
	if(_parser_in_set(state, TS_PREFIX)
			&& (string = token_get_string(state->token)) != NULL
			&& asmcode_get_arch_prefix_by_name(state->code,
				string) != NULL)
	{
		ret |= _prefix(state);
		if(_parser_in_set(state, TS_SPACE))
			ret |= _space(state);
		else
			ret |= _parser_error(state, "%s", "Expected a space");
	}
	/* instruction_name */
	ret |= _instruction_name(state);
	if(_parser_in_set(state, TS_SPACE))
	{
		/* [ space */
		ret |= _space(state);
		/* [ operand_list ] ] */
		if(_parser_in_set(state, TS_OPERAND_LIST))
			ret |= _operand_list(state);
	}
	/* call the current instruction */
	if(state->call.name != NULL)
	{
		if(asmcode_instruction(state->code, &state->call) != 0)
			ret |= _parser_error(state, "%s", error_get(NULL));
	}
	/* FIXME memory leak (register names...) */
	memset(&state->call, 0, sizeof(state->call));
	return ret;
}


/* instruction_name */
static int _instruction_name(State * state)
	/* WORD */
{
	char const * string;
	AsmArchInstruction const * ai;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if((string = token_get_string(state->token)) == NULL)
	{
		_parser_scan(state);
		return _parser_error(state, "%s", "Empty instructions are not"
				" allowed");
	}
	if((ai = asmcode_get_arch_instruction_by_name(state->code, string))
			== NULL)
		return _parser_error(state, "%s: %s", string,
				"Unknown instruction");
	state->call.name = ai->name;
	/* optimized free(state->operator); out */
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %s \"%s\"\n", __func__, "instruction ",
			string);
#endif
	return _parser_scan(state);
}


/* operand_list */
static int _operand_list(State * state)
	/* operand [ space ] { "," [ space ] operand [ space ] } */
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	ret = _operand(state);
	state->call.operands_cnt++;
	if(_parser_in_set(state, TS_SPACE))
		ret |= _space(state);
	while(_parser_is_code(state, AS_CODE_COMMA))
	{
		/* "," */
		ret |= _parser_scan(state);
		/* [ space ] */
		if(_parser_in_set(state, TS_SPACE))
			ret |= _space(state);
		/* operand */
		ret |= _operand(state);
		/* [ space ] */
		if(_parser_in_set(state, TS_SPACE))
			ret |= _space(state);
		state->call.operands_cnt++;
	}
	return ret;
}


/* operand */
static int _operand(State * state)
	/* value | address */
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(_parser_in_set(state, TS_VALUE))
		return _value(state);
	else if(_parser_in_set(state, TS_ADDRESS))
		return _address(state);
	return _parser_error(state, "%s", "Expected value or address");
}


/* prefix */
static int _prefix(State * state)
	/* WORD */
{
	AsmArchPrefix const * prefix;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* this is not supposed to fail */
	prefix = asmcode_get_arch_prefix_by_name(state->code,
			token_get_string(state->token));
	state->call.prefix = prefix->name;
	return _parser_scan(state);
}


/* value */
static int _value(State * state)
	/* symbol | register | immediate */
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(_parser_in_set(state, TS_SYMBOL))
		return _symbol(state);
	else if(_parser_in_set(state, TS_REGISTER))
		return _register(state);
	else if(_parser_in_set(state, TS_IMMEDIATE))
		return _immediate(state);
	return _parser_error(state, "%s", "Expected symbol, register or"
			" immediate value");
}


/* symbol */
static int _symbol(State * state)
	/* WORD */
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* FIXME really implement */
	return _parser_scan(state);
}


/* register */
static int _register(State * state)
	/* "%" WORD */
{
	AsmArchOperand * p;
	char const * string;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%%%s) %d\n", __func__, token_get_string(
				state->token), state->address);
#endif
	p = &state->call.operands[state->call.operands_cnt];
	string = token_get_string(state->token); /* XXX may be null or empty? */
	if(state->address == 2)
	{
		p->definition = AO_DREGISTER2(0, 0, 0, 0);
		p->value.dregister2.name = p->value.dregister.name;
		p->value.dregister2.name2 = strdup(string);
	}
	else if(state->address == 1)
	{
		p->definition = AO_DREGISTER(0, 0, 0, 0);
		p->value.dregister.name = strdup(string);
	}
	else if(state->address == 0)
	{
		p->definition = AO_REGISTER(0, 0, 0);
		p->value._register.name = strdup(string);
	}
	return _parser_scan(state);
}


/* immediate */
static int _immediate(State * state)
	/* [ sign ] "$" NUMBER */
{
	int ret = 0;
	AsmArchOperand * p;
	char const * string;
	uint64_t value = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* [ sign ] */
	if(_parser_in_set(state, TS_SIGN))
		ret |= _sign(state);
	else
		state->negative = 0;
	/* "$" NUMBER */
	p = &state->call.operands[state->call.operands_cnt];
	if((string = token_get_string(state->token)) != NULL
			|| strlen(string) == 0)
		value = strtoul(string + 1, NULL, 0);
	else
		ret = _parser_error(state, "%s", "Empty values are not"
				" allowed");
	if(state->address > 0)
		p->value.dregister.offset = value;
	else
	{
		p->value.immediate.value = strtoul(string + 1, NULL, 0);
		p->definition = AO_IMMEDIATE(0, 0, 0);
		p->value.immediate.negative = state->negative;
	}
	return ret | _parser_scan(state);
}


/* address */
static int _address(State * state)
	/* "[" [ space ] [ sign [ space ] ] value [ space ] [ offset [ space ] ] "]" */
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	state->address = 1;
	/* "[" */
	ret = _parser_scan(state);
	/* [ space ] */
	if(_parser_in_set(state, TS_SPACE))
		ret |= _space(state);
	/* [ sign [ space ] ] */
	if(_parser_in_set(state, TS_SIGN))
	{
		ret |= _sign(state);
		if(_parser_in_set(state, TS_SPACE))
			ret |= _space(state);
	}
	else
		state->negative = 0;
	/* value */
	if(_parser_in_set(state, TS_VALUE))
		ret |= _value(state);
	else
		ret |= _parser_error(state, "%s", "Expected value");
	/* [ space ] */
	if(_parser_in_set(state, TS_SPACE))
		ret |= _space(state);
	/* [ offset [ space ] ] */
	if(_parser_in_set(state, TS_OFFSET))
	{
		state->address = 2;
		ret |= _offset(state);
		if(_parser_in_set(state, TS_SPACE))
			ret |= _space(state);
	}
	state->address = 0;
	/* "]" */
	return ret | _parser_check(state, AS_CODE_OPERATOR_RBRACKET);
}


/* offset */
static int _offset(State * state)
	/* sign [ space ] value */
{
	int ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	/* sign */
	ret = _sign(state);
	/* [ space ] */
	if(_parser_in_set(state, TS_SPACE))
		ret |= _space(state);
	/* value */
	if(_parser_in_set(state, TS_VALUE))
		ret |= _value(state);
	else
		ret |= _parser_error(state, "%s", "Expected a value");
	return ret;
}


/* sign */
static int _sign(State * state)
	/* ( "+" | "-" ) */
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	state->negative = (token_get_code(state->token)
			== AS_CODE_OPERATOR_MINUS) ? 1 : 0;
	return _parser_scan(state);
}
