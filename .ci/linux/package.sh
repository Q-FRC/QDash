#!/bin/sh -e

# SPDX-FileCopyrightText: 2025 QDash
# SPDX-License-Identifier: GPL-3.0-or-later

# This script assumes you're in the source directory

URUNTIME="https://raw.githubusercontent.com/pkgforge-dev/Anylinux-AppImages/refs/heads/main/useful-tools/uruntime2appimage.sh"
SHARUN="https://raw.githubusercontent.com/pkgforge-dev/Anylinux-AppImages/refs/heads/qml/useful-tools/quick-sharun.sh"

export ICON="$PWD"/dist/org.Q-FRC.QDash.svg
export DESKTOP="$PWD"/dist/org.Q-FRC.QDash.desktop
export OPTIMIZE_LAUNCH=1

case "$1" in
    amd64|"")
        echo "Packaging amd64-v3 optimized build of QDash"
        ARCH="amd64_v3"
        ;;
    steamdeck)
        echo "Packaging Steam Deck (Zen 2) optimized build of QDash"
        ARCH="steamdeck"
        ;;
    rog-ally|allyx)
        echo "Packaging ROG Ally X (Zen 4) optimized build of QDash"
        ARCH="rog-ally-x"
        ;;
    legacy)
        echo "Packaging amd64 generic build of QDash"
        ARCH=amd64
        ;;
    aarch64)
        echo "Packaging armv8-a build of QDash"
        ARCH=aarch64
        ;;
    armv9)
        echo "Packaging armv9-a build of QDash"
        ARCH=armv9
        ;;
esac

if [ "$BUILDDIR" = '' ]
then
	BUILDDIR=build
fi

QDash_TAG=$(git describe --tags --abbrev=0)
echo "Making \"$QDash_TAG\" build"
VERSION="$QDash_TAG"

export UPINFO="gh-releases-zsync|Q-FRC|QDash|latest|*$ARCH.AppImage.zsync"
export OUTNAME=QDash-"$VERSION"-"$ARCH".AppImage

# Deploy dependencies
wget --retry-connrefused --tries=30 "$SHARUN" -O ./quick-sharun
chmod +x ./quick-sharun
./quick-sharun "$BUILDDIR"/QDash/Native/QDash

# MAKE APPIMAGE WITH URUNTIME
wget --retry-connrefused --tries=30 "$URUNTIME" -O ./uruntime2appimage
chmod +x ./uruntime2appimage
./uruntime2appimage

if [ "$DEVEL" = 'true' ]; then
    rm -f ./*.AppImage.zsync
fi

echo "All Done!"
