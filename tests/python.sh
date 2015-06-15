#!/bin/sh
#$Id$
#Copyright (c) 2014-2015 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS Devel Asm
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, version 3 of the License.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.



#variables
[ -f "../config.sh" ] && . "../config.sh"
[ -n "$PREFIX" ] || PREFIX="/usr/local"
[ -n "$LIBDIR" ] || LIBDIR="$PREFIX/lib"
PKG_CONFIG_PATH="$PWD/../data:$LIBDIR/pkgconfig"
PYTHONDIR="../src/python"
#executables
MAKE="make"


[ -n "$OBJDIR" ] && PKG_CONFIG_PATH="${OBJDIR}../data:$LIBDIR/pkgconfig"
(cd "$PYTHONDIR" && PKG_CONFIG_PATH="$PKG_CONFIG_PATH" $MAKE clean all)
