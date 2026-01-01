// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Constants

Button {
    required property string label

    icon.source: "qrc:/" + label
    icon.width: 45
    icon.height: 45
    icon.color: Constants.accent

    background: Item {}
}
