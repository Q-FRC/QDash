// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

NativeDialog {
    title: "Close Tab?"

    width: 175
    height: 100

    Text {
        font.pixelSize: 15
        color: Clover.theme.text
        text: "Are you sure you want to close this tab?"

        wrapMode: Text.WordWrap
        anchors.fill: parent
    }

    standardButtons: Dialog.Yes | Dialog.No
}
