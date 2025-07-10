#!/bin/sh -e

# SPDX-FileCopyrightText: 2025 QDash
# SPDX-License-Identifier: GPL-3.0-or-later

# This script assumes you're in the source directory

export APPIMAGE_EXTRACT_AND_RUN=1
export BASE_ARCH="$(uname -m)"

SHARUN="https://github.com/VHSgunzo/sharun/releases/latest/download/sharun-${BASE_ARCH}-aio"
URUNTIME="https://github.com/VHSgunzo/uruntime/releases/latest/download/uruntime-appimage-dwarfs-${BASE_ARCH}"

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

# NOW MAKE APPIMAGE
mkdir -p ./AppDir
cd ./AppDir

cp ../dist/org.Q-FRC.QDash.desktop .
cp ../dist/org.Q-FRC.QDash.svg .

ln -sf ./org.Q-FRC.QDash.svg ./.DirIcon

UPINFO='gh-releases-zsync|Q-FRC|QDash|latest|*.AppImage.zsync'

LIBDIR="/usr/lib"

# Workaround for Gentoo
if [ ! -d "$LIBDIR/qt6" ]
then
	LIBDIR="/usr/lib64"
fi

# Workaround for Debian
if [ ! -d "$LIBDIR/qt6" ]
then
    LIBDIR="/usr/lib/${BASE_ARCH}-linux-gnu"
fi

# Bundle all libs

wget --retry-connrefused --tries=30 "$SHARUN" -O ./sharun-aio
chmod +x ./sharun-aio
xvfb-run -a ./sharun-aio l -p -v -e -s -k \
	../$BUILDDIR/QDash/Native/QDash \
	$LIBDIR/libXss.so* \
	$LIBDIR/libdecor-0.so* \
	$LIBDIR/qt6/plugins/audio/* \
	$LIBDIR/qt6/plugins/bearer/* \
	$LIBDIR/qt6/plugins/imageformats/* \
	$LIBDIR/qt6/plugins/iconengines/* \
	$LIBDIR/qt6/plugins/platforms/* \
	$LIBDIR/qt6/plugins/platformthemes/* \
	$LIBDIR/qt6/plugins/platforminputcontexts/* \
	$LIBDIR/qt6/plugins/styles/* \
	$LIBDIR/qt6/plugins/xcbglintegrations/* \
	$LIBDIR/qt6/plugins/wayland-*/*

rm -f ./sharun-aio

# Copy QML Files
mkdir -p shared/lib/qt6/qml
set +e
cp -r $LIBDIR/qt6/qml/Qt{,Core,Multimedia,Network,Quick} shared/lib/qt6/qml/
set -e

# Prepare sharun
if [ "$ARCH" = 'aarch64' ]; then
	# allow the host vulkan to be used for aarch64 given the sad situation
	echo 'SHARUN_ALLOW_SYS_VKICD=1' > ./.env
fi

# Workaround for Gentoo
if [ -d "shared/libproxy" ]; then
	cp shared/libproxy/* lib/
fi

ln -f ./sharun ./AppRun
./sharun -g

# turn appdir into appimage
cd ..
wget -q "$URUNTIME" -O ./uruntime
chmod +x ./uruntime

#Add udpate info to runtime
echo "Adding update information \"$UPINFO\" to runtime..."
./uruntime --appimage-addupdinfo "$UPINFO"

echo "Generating AppImage..."
./uruntime --appimage-mkdwarfs -f \
	--set-owner 0 --set-group 0 \
	--no-history --no-create-timestamp \
	--compression zstd:level=22 -S26 -B32 \
	--header uruntime \
    -N 4 \
	-i ./AppDir -o QDash-"$VERSION"-"$ARCH".AppImage

	# --categorize=hotness --hotness-list=.ci/linux/QDash.dwfsprof \
if [ "$DEVEL" != 'true' ]; then
    echo "Generating zsync file..."
    zsyncmake *.AppImage -u *.AppImage
fi
echo "All Done!"
