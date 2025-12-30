// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Constants

NativeDialog {
    id: dialog
    required property string text

    Text {
        color: Constants.palette.text
        font.pixelSize: (14)

        text: dialog.text

        wrapMode: Text.Wrap
        textFormat: Text.RichText

        anchors.fill: parent

        onLinkActivated: link => Qt.openUrlExternally(link)
    }
}
