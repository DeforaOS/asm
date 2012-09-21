#!/usr/bin/env sh
#$Id$
#Copyright (c) 2012 Pierre Pronchery <khorben@defora.org>
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
DEASM="../src/deasm-static"
DEBUG="debug"


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
	$DEBUG $cmd "$file"
}


#debug
debug()
{
	echo "$@" 1>&2
	"$@"
}


#usage
_usage()
{
	echo "Usage: tests.sh" 1>&2
	return 1
}


#main
while getopts "P:" "name"; do
	case "$name" in
		P)
			#XXX ignored
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -ne 1 ]; then
	_usage
	exit $?
fi
target="$1"

> "$target"
FAILED=
deasm amd64		>> "$target"	|| FAILED="$FAILED amd64(error $?)"
deasm arm		>> "$target"	|| FAILED="$FAILED arm(error $?)"
deasm armeb		>> "$target"	|| FAILED="$FAILED armeb(error $?)"
deasm armel		>> "$target"	|| FAILED="$FAILED armel(error $?)"
deasm dalvik flat	>> "$target"	|| FAILED="$FAILED dalvik(error $?)"
deasm i386		>> "$target"	|| FAILED="$FAILED i386(error $?)"
deasm i386_real flat	>> "$target"	|| FAILED="$FAILED i386_flat(error $?)"
deasm i486		>> "$target"	|| FAILED="$FAILED i486(error $?)"
deasm i586		>> "$target"	|| FAILED="$FAILED i586(error $?)"
deasm i686		>> "$target"	|| FAILED="$FAILED i686(error $?)"
deasm java flat		>> "$target"	|| FAILED="$FAILED java(error $?)"
deasm sparc		>> "$target"	|| FAILED="$FAILED sparc(error $?)"
deasm sparc64		>> "$target"	|| FAILED="$FAILED sparc64(error $?)"
deasm yasep flat	>> "$target"	|| FAILED="$FAILED yasep(error $?)"
deasm yasep16 flat	>> "$target"	|| FAILED="$FAILED yasep16(error $?)"
deasm yasep32 flat	>> "$target"	|| FAILED="$FAILED yasep32(error $?)"
[ -z "$FAILED" ]			&& exit 0
echo "Failed tests:$FAILED" 1>&2
#XXX ignore errors for now
#exit 2
