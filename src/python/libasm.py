#!/usr/bin/env python2.7
#$Id$
#Copyright (c) 2013 Pierre Pronchery <khorben@defora.org>
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
