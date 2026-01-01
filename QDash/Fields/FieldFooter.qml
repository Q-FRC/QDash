// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Constants

Rectangle {
    property bool valid

    height: 2
    color: valid ? Constants.palette.text : "red"
    width: parent.width

    anchors {
        bottom: parent.bottom
        left: parent.left
    }

    Behavior on color {
        ColorAnimation {
            duration: 250
        }
    }
}
