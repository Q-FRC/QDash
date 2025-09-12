#!/bin/bash -e

# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

if [ "$USE_CCACHE" = "true" ]; then
    export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_C_COMPILER_LAUNCHER=ccache)
fi

if [ "$WINDEPLOYQT" == "" ]; then
    echo "You must supply the WINDEPLOYQT environment variable."
    exit 1
fi

if [ "$BUILD_TYPE" = "" ]; then
    export BUILD_TYPE="RelWithDebInfo"
fi

export EXTRA_CMAKE_FLAGS=("${EXTRA_CMAKE_FLAGS[@]}" $@)

mkdir -p build && cd build
cmake .. -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBUILD_SHARED_LIBS=OFF \
	  "${EXTRA_CMAKE_FLAGS[@]}"

ninja

if [ "$USE_CCACHE" = "true" ]; then
    ccache -s
fi

set +e
rm -f build/*.pdb
set -e

$WINDEPLOYQT --release \
             --no-compiler-runtime \
             --no-opengl-sw \
             --no-system-dxc-compiler \
             --no-system-d3d-compiler \
             --dir pkg \
             --qmldir ../QDash \
             --qmlimport $QML2_IMPORT_PATH \
             QDash.exe

cp QDash.exe pkg
