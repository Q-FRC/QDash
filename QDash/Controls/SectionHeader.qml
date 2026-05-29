// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover

Label {
    required property string label
    font.pixelSize: 18
    font.weight: 700
    text: label

    width: parent.width
    horizontalAlignment: Text.AlignLeft
}
