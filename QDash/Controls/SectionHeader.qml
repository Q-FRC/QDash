// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QtQuick
import QtQuick.Controls

Label {
    required property string label

    font.pixelSize: 18
    font.weight: 700
    horizontalAlignment: Text.AlignLeft
    text: label
    width: parent.width
}
