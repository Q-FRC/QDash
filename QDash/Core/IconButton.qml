// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QtQuick
import QtQuick.Controls

Button {
    required property string label

    icon.color: Clover.theme.currentAccent
    icon.height: 45
    icon.source: "qrc:/" + label
    icon.width: 45

    background: Item {}
}
