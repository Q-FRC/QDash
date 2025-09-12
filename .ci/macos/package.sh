#!/bin/sh -ex

# SPDX-FileCopyrightText: 2025 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

cd build
rm -rf QDash.app
cp -r QDash/Native/QDash.app .
chmod a+x QDash.app/Contents/MacOS/QDash
mkdir -p QDash.app/Contents/Resources
cp ../dist/QDash.icns QDash.app/Contents/Resources
cp ../dist/App.entitlements QDash.app/Contents/Resources

unset DYLD_LIBRARY_PATH
unset DYLD_FRAMEWORK_PATH

macdeployqt QDash.app \
            -qmldir=../QDash \
            -qmlimport=$QML_SOURCES_PATHS \
            -verbose=2

macdeployqt QDash.app \
            -qmldir=../QDash \
            -qmlimport=$QML_SOURCES_PATHS \
            -verbose=2 \
            -always-overwrite

APP=QDash.app

# FixMachOLibraryPaths
find "$APP/Contents/Frameworks" ""$APP/Contents/MacOS"" -type f \( -name "*.dylib" -o -perm +111 \) | while read file; do
    if file "$file" | grep -q "Mach-O"; then
        otool -L "$file" | awk '/@rpath\// {print $1}' | while read lib; do
            lib_name="${lib##*/}"
            new_path="@executable_path/../Frameworks/$lib_name"
            install_name_tool -change "$lib" "$new_path" "$file"
        done

        if [[ "$file" == *.dylib ]]; then
            lib_name="${file##*/}"
            new_id="@executable_path/../Frameworks/$lib_name"
            install_name_tool -id "$new_id" "$file"
        fi
    fi
done

# TODO: sign w/ real identity?
codesign --force --deep --entitlements ../dist/App.entitlements -s - QDash.app
tar czf ../QDash.tar.gz QDash.app
