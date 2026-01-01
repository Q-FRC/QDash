// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Constants

Rectangle {
    color: Constants.accent
    topLeftRadius: 20
    topRightRadius: 20

    height: 40
    width: parent.width

    Text {
        id: txt

        text: "Configure Widget"
        font.pixelSize: 24
        font.bold: true

        color: "white"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors {
            fill: parent
            margins: 8
        }
    }
}
