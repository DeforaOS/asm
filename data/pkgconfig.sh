#!/bin/sh
#$Id$



#variables
. "../config.sh"
DEBUG="_debug"
INSTALL="install -m 0644"
MKDIR="mkdir -p"
RM="rm -f"
SED="sed"


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
	echo "Usage: pkgconfig.sh [-i|-u][-P prefix] target" 1>&2
	return 1
}


#main
args=`getopt iuP: $*`
if [ $? -ne 0 ]; then
	_usage
	exit $?
fi
set -- $args
install=0
uninstall=0
while [ $# -gt 0 ]; do
	case "$1" in
		-i)
			install=1
			;;
		-u)
			uninstall=1
			;;
		-P)
			PREFIX="$2"
			shift
			;;
		--)
			shift
			break
			;;
	esac
	shift
done

PKGCONFIG="$PREFIX/lib/pkgconfig"
while [ $# -gt 0 ]; do
	target="$1"
	shift

	#uninstall
	if [ "$uninstall" -eq 1 ]; then
		$DEBUG $RM "$PKGCONFIG/$target"			|| exit 2
		continue
	fi

	#install
	if [ "$install" -eq 1 ]; then
		$DEBUG $MKDIR "$PKGCONFIG"			|| exit 2
		$DEBUG $INSTALL "$target" "$PKGCONFIG/$target"	|| exit 2
		continue
	fi

	#create
	$SED -e "s,PREFIX,$PREFIX," -e "s,VERSION,$VERSION," "$target.in" \
		> "$target"					|| exit 2
done
