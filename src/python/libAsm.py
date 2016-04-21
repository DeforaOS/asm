#!/usr/bin/env python2.7
#$Id$
#Copyright (c) 2013-2014 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS Devel asm
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU Lesser General Public License as published by
#the Free Software Foundation, version 3 of the License.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Lesser General Public License for more details.
#
#You should have received a copy of the GNU Lesser General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.



import _libasm


#Asm
class Asm:
    def __init__(self, arch, format):
        self.asm = _libasm.asm_new(arch, format)

    def get_arch(self):
        return _libasm.asm_get_arch(self.asm)

    def get_format(self):
        return _libasm.asm_get_format(self.asm)

    def set_arch(self, arch):
        return _libasm.asm_set_arch(self.asm, arch)

    def set_format(self, format):
        return _libasm.asm_set_format(self.asm, format)

    def guess_arch(self):
        return _libasm.asm_guess_arch(self.asm)

    def guess_format(self):
        return _libasm.asm_guess_format(self.asm)

    def close(self):
        return _libasm.asm_close(self.asm)

    def assemble_string(self, outfile, string):
        return _libasm.asm_assemble_string(self.asm, outfile, string)

    def open_assemble(self, outfile):
        return _libasm.asm_open_assemble(self.asm, outfile)

    def instruction(self, name):
        return _libasm.asm_instruction(self.asm, name)

    def open_deassemble(self, filename, raw):
        return _libasm.asm_open_deassemble(self.asm, filename, raw)
