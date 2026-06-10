#!/bin/sh -e

# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

find QDash -name '*.cpp' -o -name '*.h' | xargs clang-format -i -style=file:QDash/.clang-format

find QDash -name '*.qml' | xargs /usr/lib64/qt6/bin/qmlformat -i --ignore-settings \
	-W 120 -n --objects-spacing --functions-spacing --semicolon-rule essential -S \
	--single-line-empty-objects --group-attributes-together
