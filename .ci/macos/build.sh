#!/bin/bash -e

# SPDX-FileCopyrightText: 2025 eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

if [ "$USE_CCACHE" = "true" ]; then
    export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" -DUSE_CCACHE=ON)
fi

if [ "$BUILD_TYPE" = "" ]; then
    export BUILD_TYPE="RelWithDebInfo"
fi

export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" $@)

mkdir -p build && cd build
cmake .. -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_OSX_ARCHITECTURES="arm64" \
	-DQDASH_WEBVIEW=OFF \
    -DBUILD_SHARED_LIBS=OFF \
	"${EXTRA_CMAKE_FLAGS[@]}"

ninja

ccache -s
