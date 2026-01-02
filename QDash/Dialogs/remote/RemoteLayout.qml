
// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick

import Carboxyl.Clover

Rectangle {
    id: rem

    signal clicked
    signal activated

    // color: mouseArea.containsMouse ? "#82bbff" : (ListView.isCurrentItem ? "#00aaff" : Constants.palette.dialogBg)
    radius: 5
    opacity: 1

    Behavior on color {
        ColorAnimation {
            duration: 250
        }
    }

    border {
        color: Clover.theme.text
        width: 1
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onDoubleClicked: activated()

        onClicked: rem.clicked()
    }

    Text {
        anchors.fill: parent
        anchors.leftMargin: 20

        color: Clover.theme.text
        text: model.name
        font.pixelSize: Math.round(18)

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
