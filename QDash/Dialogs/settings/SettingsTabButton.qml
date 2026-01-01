// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QDash.Constants
import QDash.Items

TabButton {
    required property string label

    id: button

    contentItem: ColumnLayout {
        IconButton {
            label: button.label

            Layout.maximumHeight: 75
            Layout.maximumWidth: 75

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            onClicked: button.clicked()
        }

        Text {
            font.pixelSize: Math.round(18)
            text: label

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            color: Constants.palette.text
        }
    }

    background: Rectangle {
        // implicitWidth: parent.width
        topLeftRadius: 12
        topRightRadius: 12
        color: "transparent"
    }
}
