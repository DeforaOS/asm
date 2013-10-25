#!/usr/bin/env sh
#$Id$
#Copyright (c) 2012-2013 Pierre Pronchery <khorben@defora.org>
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
DATE="date"
DEASM="../src/deasm-static"
DEBUG="debug"


#functions
#deasm
_deasm()
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
	echo "Usage: tests.sh [-c][-P prefix]" 1>&2
	return 1
}


#main
clean=0
while getopts "cP:" "name"; do
	case "$name" in
		c)
			clean=1
			;;
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

[ "$clean" -ne 0 ]			&& exit 0

$DATE > "$target"
FAILED=
_deasm amd64		>> "$target"	|| FAILED="$FAILED amd64(error $?)"
_deasm arm		>> "$target"	|| FAILED="$FAILED arm(error $?)"
_deasm armeb		>> "$target"	|| FAILED="$FAILED armeb(error $?)"
_deasm armel		>> "$target"	|| FAILED="$FAILED armel(error $?)"
_deasm dalvik flat	>> "$target"	|| FAILED="$FAILED dalvik(error $?)"
_deasm i386		>> "$target"	|| FAILED="$FAILED i386(error $?)"
_deasm i386_real flat	>> "$target"	|| FAILED="$FAILED i386_flat(error $?)"
_deasm i486		>> "$target"	|| FAILED="$FAILED i486(error $?)"
_deasm i586		>> "$target"	|| FAILED="$FAILED i586(error $?)"
_deasm i686		>> "$target"	|| FAILED="$FAILED i686(error $?)"
_deasm java flat	>> "$target"	|| FAILED="$FAILED java(error $?)"
_deasm sparc		>> "$target"	|| FAILED="$FAILED sparc(error $?)"
_deasm sparc64		>> "$target"	|| FAILED="$FAILED sparc64(error $?)"
_deasm yasep flat	>> "$target"	|| FAILED="$FAILED yasep(error $?)"
_deasm yasep16 flat	>> "$target"	|| FAILED="$FAILED yasep16(error $?)"
_deasm yasep32 flat	>> "$target"	|| FAILED="$FAILED yasep32(error $?)"
[ -z "$FAILED" ]			&& exit 0
echo "Failed tests:$FAILED" 1>&2
#XXX ignore errors for now
#exit 2
