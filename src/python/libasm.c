/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Devel asm */
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



#include <Python.h>
#include "Devel/Asm.h"


/* libasm */
/* private */
/* constants */
static char const _libasm_asm_name[] = "libasm::Asm";


/* prototypes */
/* Asm */
static PyObject * _libasm_asm_new(PyObject * self, PyObject * args);
static void _libasm_asm_delete(PyObject * self);

/* accessors */
static PyObject * _libasm_asm_get_arch(PyObject * self, PyObject * args);
static PyObject * _libasm_asm_get_format(PyObject * self, PyObject * args);
static PyObject * _libasm_asm_set_arch(PyObject * self, PyObject * args);
static PyObject * _libasm_asm_set_format(PyObject * self, PyObject * args);

/* useful */
/* detection */
static PyObject * _libasm_asm_guess_arch(PyObject * self, PyObject * args);
static PyObject * _libasm_asm_guess_format(PyObject * self, PyObject * args);

/* common */
static PyObject * _libasm_asm_close(PyObject * self, PyObject * args);

/* assemble */
static PyObject * _libasm_asm_assemble_string(PyObject * self, PyObject * args);
static PyObject * _libasm_asm_open_assemble(PyObject * self, PyObject * args);

static PyObject * _libasm_asm_instruction(PyObject * self, PyObject * args);

/* deassemble */
static PyObject * _libasm_asm_open_deassemble(PyObject * self, PyObject * args);


/* variables */
static PyMethodDef _libasm_methods[] =
{
	{ "asm_new", _libasm_asm_new, METH_VARARGS,
		"Instantiates an Asm object." },
	{ "asm_get_arch", _libasm_asm_get_arch, METH_VARARGS,
		"Obtain the current architecture in use." },
	{ "asm_get_format", _libasm_asm_get_format, METH_VARARGS,
		"Obtain the current format in use." },
	{ "asm_set_arch", _libasm_asm_set_arch, METH_VARARGS,
		"Set the current architecture in use." },
	{ "asm_set_format", _libasm_asm_set_format, METH_VARARGS,
		"Set the current format in use." },
	{ "asm_guess_arch", _libasm_asm_guess_arch, METH_VARARGS,
		"Guess the current architecture." },
	{ "asm_guess_format", _libasm_asm_guess_format, METH_VARARGS,
		"Guess the current format." },
	{ "asm_close",	_libasm_asm_close, METH_VARARGS,
		"Close the file currently opened." },
	{ "asm_assemble_string", _libasm_asm_assemble_string, METH_VARARGS,
		"Write instructions from a string directly to a file." },
	{ "asm_open_assemble", _libasm_asm_open_assemble, METH_VARARGS,
		"Open a file to write instructions." },
	{ "asm_instruction", _libasm_asm_instruction, METH_VARARGS,
		"Process an assembly instruction." },
	{ "asm_open_deassemble", _libasm_asm_open_deassemble, METH_VARARGS,
		"Open a file to read instructions." },
	{ NULL, NULL, 0, NULL }
};


/* public */
/* prototypes */
PyMODINIT_FUNC init_libasm(void);


/* functions */
PyMODINIT_FUNC init_libasm(void)
{
	Py_InitModule("_libasm", _libasm_methods);
}


/* private */
/* functions */
/* Asm */
/* libasm_asm_new */
static PyObject * _libasm_asm_new(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * arch;
	char const * format;

	if(!PyArg_ParseTuple(args, "ss", &arch, &format))
		return NULL;
	if((a = asm_new(arch, format)) == NULL)
		return NULL;
	return PyCapsule_New(a, _libasm_asm_name, _libasm_asm_delete);
}


/* libasm_asm_delete */
static void _libasm_asm_delete(PyObject * self)
{
	Asm * a;

	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return;
	asm_delete(a);
}


/* accessors */
/* libasm_asm_get_arch */
static PyObject * _libasm_asm_get_arch(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * ret;

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL
			|| (ret = asm_get_arch(a)) == NULL)
		return NULL;
	return Py_BuildValue("s", ret);
}


/* libasm_asm_get_format */
static PyObject * _libasm_asm_get_format(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * ret;

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL
			|| (ret = asm_get_format(a)) == NULL)
		return NULL;
	return Py_BuildValue("s", ret);
}


/* libasm_asm_set_arch */
static PyObject * _libasm_asm_set_arch(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * arch;
	int ret;

	if(!PyArg_ParseTuple(args, "Os", &self, &arch))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_set_arch(a, arch);
	return Py_BuildValue("i", ret);
}


/* libasm_asm_set_format */
static PyObject * _libasm_asm_set_format(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * format;
	int ret;

	if(!PyArg_ParseTuple(args, "Os", &self, &format))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_set_format(a, format);
	return Py_BuildValue("i", ret);
}


/* useful */
/* detection */
/* libasm_asm_guess_arch */
static PyObject * _libasm_asm_guess_arch(PyObject * self, PyObject * args)
{
	Asm * a;
	int ret;

	if(!PyArg_ParseTuple(args, "O", self))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_guess_arch(a);
	return Py_BuildValue("i", ret);
}


/* libasm_guess_format */
static PyObject * _libasm_asm_guess_format(PyObject * self, PyObject * args)
{
	Asm * a;
	int ret;

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_guess_format(a);
	return Py_BuildValue("i", ret);
}


/* common */
/* libasm_asm_close */
static PyObject * _libasm_asm_close(PyObject * self, PyObject * args)
{
	Asm * a;
	int ret;

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_close(a);
	return Py_BuildValue("i", ret);
}


/* assemble */
/* libasm_asm_assemble_string */
static PyObject * _libasm_asm_assemble_string(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * outfile;
	char const * string;
	int ret;

	if(!PyArg_ParseTuple(args, "Oss", &self, &outfile, &string))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_assemble_string(a, NULL, outfile, string);
	return Py_BuildValue("i", ret);
}


/* libasm_asm_open_assemble */
static PyObject * _libasm_asm_open_assemble(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * outfile;
	int ret;

	if(!PyArg_ParseTuple(args, "Os", &self, &outfile))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_open_assemble(a, outfile);
	return Py_BuildValue("i", ret);
}


/* libasm_asm_instruction */
static PyObject * _libasm_asm_instruction(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * name;
	int ret;

	/* FIXME really parse the arguments */
	if(!PyArg_ParseTuple(args, "Os", &self, &name))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_instruction(a, name, 0);
	return Py_BuildValue("i", ret);
}


/* deassemble */
/* libasm_asm_open_deassemble */
static PyObject * _libasm_asm_open_deassemble(PyObject * self, PyObject * args)
{
	Asm * a;
	char const * filename;
	int raw;
	AsmCode * ret;

	if(!PyArg_ParseTuple(args, "Osi", &self, &filename, &raw))
		return NULL;
	if((a = PyCapsule_GetPointer(self, _libasm_asm_name)) == NULL)
		return NULL;
	ret = asm_open_deassemble(a, filename, raw);
	/* FIXME really return an AsmCode Python object */
	return Py_BuildValue("p", ret);
}
