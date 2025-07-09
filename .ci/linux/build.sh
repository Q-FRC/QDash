#!/bin/bash -e

# SPDX-FileCopyrightText: 2025 QDash
# SPDX-License-Identifier: GPL-3.0-or-later

export ARCH="$(uname -m)"

case "$1" in
    amd64|"")
        echo "Making amd64-v3 optimized build of QDash"
        ARCH="amd64_v3"
        ARCH_FLAGS="-march=x86-64-v3"
        export USE_CCACHE=true
        ;;
    steamdeck)
        echo "Making Steam Deck (Zen 2) optimized build of QDash"
        ARCH="steamdeck"
        ARCH_FLAGS="-march=znver2 -mtune=znver2"
        export USE_CCACHE=true
        ;;
    rog-ally|allyx)
        echo "Making ROG Ally X (Zen 4) optimized build of QDash"
        ARCH="rog-ally-x"
        ARCH_FLAGS="-march=znver3 -mtune=znver4" # GH actions runner is a Zen 3 CPU, so a small workaround
        export USE_CCACHE=true
        ;;
    legacy)
        echo "Making amd64 generic build of QDash"
        ARCH=amd64
        ARCH_FLAGS="-march=x86-64 -mtune=generic"
        export USE_CCACHE=true
        ;;
    aarch64)
        echo "Making armv8-a build of QDash"
        ARCH=aarch64
        ARCH_FLAGS="-march=armv8-a -mtune=generic -w"
        ;;
    armv9)
        echo "Making armv9-a build of QDash"
        ARCH=armv9
        ARCH_FLAGS="-march=armv9-a -mtune=generic -w"
        ;;
esac

export ARCH_FLAGS="$ARCH_FLAGS -O2"

NPROC="$2"
if [ -z "$NPROC" ]; then
    NPROC="$(nproc)"
else
    shift
fi

[ "$1" != "" ] && shift

export EXTRA_CMAKE_FLAGS=(-DBUILD_SHARED_LIBS=OFF)

if [ "$TARGET" = "appimage" ]; then
    export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" -DCMAKE_INSTALL_PREFIX=/usr)
fi

if [ "$USE_CCACHE" = "true" ]; then
    export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_C_COMPILER_LAUNCHER=ccache)
fi

if [ "$BUILD_TYPE" = "" ]; then
    export BUILD_TYPE="RelWithDebInfo"
fi

export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" $@)

mkdir -p build
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_FLAGS="$ARCH_FLAGS" \
    -DCMAKE_C_FLAGS="$ARCH_FLAGS" \
	"${EXTRA_CMAKE_FLAGS[@]}"

cd build

ninja -j${NPROC}

strip -s src/native/QDash
