// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QtQuick
import QtQuick.Controls

Rectangle {
    id: rem

    signal activated
    signal clicked

    color: mouseArea.containsMouse ? "#82bbff" : (ListView.isCurrentItem ? "#00aaff" : Clover.theme.base)
    opacity: 1
    radius: 5

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

        onClicked: rem.clicked()
        onDoubleClicked: activated()
    }

    Label {
        anchors.fill: parent
        anchors.leftMargin: 20
        font.pixelSize: Math.round(18)
        horizontalAlignment: Text.AlignLeft
        text: model.name
        verticalAlignment: Text.AlignVCenter
    }
}
