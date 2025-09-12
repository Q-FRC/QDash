#!/bin/sh

# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

QDash_TAG=$(git describe --tags --abbrev=0)
echo "Making \"$QDash_TAG\" build"
VERSION="$QDash_TAG"

ZIP_NAME="QDash-Windows-${ARCH}-${VERSION}.zip"

mkdir -p artifacts
mkdir -p pack

cp -r build/pkg/* pack

cp LICENSE* README* pack/

7z a -tzip artifacts/$ZIP_NAME pack/*