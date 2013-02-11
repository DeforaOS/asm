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


/* variables */
static PyMethodDef _libasm_methods[] =
{
	{ "asm_new", _libasm_asm_new, METH_VARARGS,
		"Instantiates an Asm object." },
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
