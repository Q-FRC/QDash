// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Constants
import QDash.Dialogs

NativeDialog {
    title: "Close Tab?"

    width: 250
    height: 185

    anchors.centerIn: Overlay.overlay

    Text {
        font.pixelSize: Math.round(15)
        color: Constants.palette.text
        text: "Are you sure you want to close this tab?"

        wrapMode: Text.WordWrap
        anchors.fill: parent
    }

    standardButtons: Dialog.Yes | Dialog.No
}
