#!/bin/sh -e

# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: LGPL-3.0-or-later

RETURN=0

usage() {
	cat <<EOF
Usage: cpmutil.sh package [command]

Operate on a package or packages.

Commands:
    hash    	Verify the hash of a package, and update it if needed
    update  	Check for updates for a package
    fetch   	Fetch a package and place it in the cache
    add     	Add a new package
    rm      	Remove a package
    version 	Change the version of a package
    which   	Find which cpmfile a package is defined in
    download 	Get the download URL for a package

EOF

	exit $RETURN
}

SCRIPTS=$(CDPATH='' cd -- "$(dirname -- "$0")/package" && pwd)
export SCRIPTS

while :; do
	case "$1" in
	hash)
		shift
		"$SCRIPTS"/hash.sh "$@"
		break
		;;
	update)
		shift
		"$SCRIPTS"/update.sh "$@"
		break
		;;
	fetch)
		shift
		"$SCRIPTS"/fetch.sh "$@"
		break
		;;
	add)
		shift
		"$SCRIPTS"/add.sh "$@"
		break
		;;
	rm)
		shift
		"$SCRIPTS"/rm.sh "$@"
		break
		;;
	version)
		shift
		"$SCRIPTS"/version.sh "$@"
		break
		;;
	which)
		shift
		"$SCRIPTS"/which.sh "$@"
		break
		;;
	download)
		shift
		"$SCRIPTS"/download.sh "$@"
		break
		;;
	-h | --help) usage ;;
	"") usage ;;
	*) usage ;;
	esac

	shift
done
