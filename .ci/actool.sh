#!/bin/sh -e

# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

_icon=dist/QDash.icon

xcrun actool "$_icon" \
    --compile dist \
    --platform macosx \
    --minimum-deployment-target 11.0 \
    --app-icon QDash \
    --output-partial-info-plist /dev/null
