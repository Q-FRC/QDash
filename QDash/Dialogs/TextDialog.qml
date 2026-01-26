// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

NativeDialog {
    id: dialog
    required property string text

    Label {
        font.pixelSize: 14

        text: dialog.text

        wrapMode: Text.Wrap
        textFormat: Text.RichText

        anchors.fill: parent

        onLinkActivated: link => Qt.openUrlExternally(link)
    }
}
