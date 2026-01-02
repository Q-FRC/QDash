// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Carboxyl.Clover

Button {
    required property string label

    icon.source: "qrc:/" + label
    icon.width: 45
    icon.height: 45
    icon.color: Clover.theme.currentAccent

    background: Item {}
}
