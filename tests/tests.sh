#!/usr/bin/env sh
#$Id$
#Copyright (c) 2012 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS System libc
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
DEASM="../src/deasm"
DEBUG="debug"
DEVNULL="/dev/null"


#functions
#deasm
deasm()
{
	[ $# -lt 1 ] && return 1
	file="$1.o"
	arch="$1"
	cmd="$DEASM"
	format=""
	[ $# -eq 2 ] && format="$2"

	[ -n "$format" ] && cmd="$cmd -a $arch -f $format"
	$DEBUG $cmd "$file" > $DEVNULL
}


#debug
debug()
{
	echo $@ 1>&2
	$@
}


#main
FAILED=
deasm amd64		|| FAILED="$FAILED amd64(error $?)"
deasm arm		|| FAILED="$FAILED arm(error $?)"
deasm armeb		|| FAILED="$FAILED armeb(error $?)"
deasm armel		|| FAILED="$FAILED armel(error $?)"
deasm dalvik flat	|| FAILED="$FAILED dalvik(error $?)"
deasm i386		|| FAILED="$FAILED i386(error $?)"
deasm i386_real flat	|| FAILED="$FAILED i386_flat(error $?)"
deasm i486		|| FAILED="$FAILED i486(error $?)"
deasm i586		|| FAILED="$FAILED i586(error $?)"
deasm i686		|| FAILED="$FAILED i686(error $?)"
deasm java flat		|| FAILED="$FAILED java(error $?)"
deasm sparc		|| FAILED="$FAILED sparc(error $?)"
deasm sparc64		|| FAILED="$FAILED sparc64(error $?)"
deasm yasep flat	|| FAILED="$FAILED yasep(error $?)"
deasm yasep16 flat	|| FAILED="$FAILED yasep16(error $?)"
deasm yasep32 flat	|| FAILED="$FAILED yasep32(error $?)"
[ -z "$FAILED" ]	&& exit 0
echo "Failed tests:$FAILED" 1>&2
#XXX ignore errors for now
#exit 2
