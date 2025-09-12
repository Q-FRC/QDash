#!/bin/bash -e

# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

if [ "$USE_CCACHE" = "true" ]; then
    export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_C_COMPILER_LAUNCHER=ccache)
fi

if [ "$BUILD_TYPE" = "" ]; then
    export BUILD_TYPE="RelWithDebInfo"
fi

export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" $@)

mkdir -p build && cd build
cmake .. -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
	-DQDASH_WEBVIEW=OFF \
    -DBUILD_SHARED_LIBS=OFF \
	"${EXTRA_CMAKE_FLAGS[@]}"

ninja

ccache -s
