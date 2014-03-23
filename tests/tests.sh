#!/bin/sh
#$Id$
#Copyright (c) 2012-2014 Pierre Pronchery <khorben@defora.org>
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


#test
_test()
{
	test="$1"
	arch="$2"

	shift
	echo -n "$test:" 1>&2
	echo
	echo "Testing: $test" "$@"
	"$test" "$@" 2>&1
	res=$?
	if [ $res -ne 0 ]; then
		echo " FAILED" 1>&2
		FAILED="$FAILED $test($arch, error $res)"
		return 2
	fi
	echo " $arch PASS" 1>&2
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

FAILED=
($DATE
_test _deasm amd64
_test _deasm arm
_test _deasm armeb
_test _deasm armel
_test _deasm dalvik flat
_test _deasm i386
_test _deasm i386_real flat
_test _deasm i486
_test _deasm i586
_test _deasm i686
_test _deasm java flat
_test _deasm sparc
_test _deasm sparc64
_test _deasm template flat
_test _deasm yasep flat
_test _deasm yasep16 flat
_test _deasm yasep32 flat) > "$target"
if [ -n "$FAILED" ]; then
	echo "Failed tests:$FAILED" 1>&2
	exit 2
fi
echo "All tests completed" 1>&2
