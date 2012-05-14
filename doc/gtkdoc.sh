#!/bin/sh
#$Id$
#Copyright (c) 2012 Pierre Pronchery <khorben@defora.org>
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#TODO:
#- implement installing and uninstalling



#variables
PREFIX="/usr/local"
. "../config.sh"
DEBUG="_debug"
GTKDOC_FIXXREF="gtkdoc-fixxref"
GTKDOC_MKDB="gtkdoc-mkdb"
GTKDOC_MKHTML="gtkdoc-mkhtml"
GTKDOC_SCAN="gtkdoc-scan"
INSTALL="install -m 0644"
MKDIR="mkdir -p"
MODULE="$PACKAGE"
RM="rm -f"


#functions
#debug
_debug()
{
	echo $@
	$@
}


#usage
_usage()
{
	echo "Usage: gtkdoc.sh [-i|-u][-P prefix] target" 1>&2
	return 1
}


#main
install=0
uninstall=0
while getopts "iuP:" "name"; do
	case "$name" in
		i)
			uninstall=0
			install=1
			;;
		u)
			install=0
			uninstall=1
			;;
		P)
			PREFIX="$2"
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -eq 0 ]; then
	_usage
	exit $?
fi

[ -z "$DATADIR" ] && DATADIR="$PREFIX/share"

while [ $# -gt 0 ]; do
	target="$1"
	shift

	#create
	#determine the type
	ext="${target##*.}"
	ext="${ext#.}"
	case "$ext" in
		html)
			$MKDIR "html" &&
			(cd "html" &&
				$DEBUG $GTKDOC_MKHTML "$MODULE" \
					"../gtkdoc/$MODULE-docs.xml") &&
			(cd "gtkdoc" &&
				$DEBUG $GTKDOC_FIXXREF \
					--module="$MODULE" \
					--module-dir="../html" \
					--html-dir="$DATADIR/doc/$MODULE/html")
			;;
		stamp)
			(cd "gtkdoc" &&
				$DEBUG $GTKDOC_MKDB \
					--module="$MODULE" \
					--output-dir="xml" \
					--output-format="xml")
			;;
		types)
			$DEBUG $GTKDOC_SCAN \
				--module="$MODULE" \
				--source-dir="../include" \
				--source-dir="../src" \
				--output-dir="gtkdoc"
			;;
		*)
			echo "$0: $target: Unknown type" 1>&2
			exit 2
			;;
	esac
	#XXX ignore errors
	if [ $? -ne 0 ]; then
		echo "$0: $target: Could not create documentation" 1>&2
		install=0
	fi
done
