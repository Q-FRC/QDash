// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Carboxyl.Clover

Text {
    required property string label
    font.pixelSize: 18
    font.weight: 700
    color: Clover.theme.text
    text: label

    width: parent.width
    horizontalAlignment: Text.AlignLeft
}
