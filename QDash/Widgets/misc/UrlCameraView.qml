// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets

import QtMultimedia
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

// TODO: rotation, flip, etc
BaseWidget {
    id: widget

    readOnly: true
    roleString: "urlCamera"

    propertyKeys: ["url"]
    property string url: ""

    menuExtension: Component {
        MenuItem {
            id: reconnItem

            text: "Reconnect"

            onTriggered: player.reconnect()
        }
    }

    Rectangle {
        id: rct

        color: "transparent"

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 8
            right: parent.right
            top: titleField.bottom
        }

        Timer {
            id: connectTimer

            interval: 100
            repeat: false

            onTriggered: player.play()
        }

        MediaPlayer {
            id: player

            function reconnect() {
                player.stop()
                connectTimer.start()
            }

            source: url
            videoOutput: video

            onErrorOccurred: (error, errorString) => {
                logs.warn("UrlCameraView", "Qt reported error " + errorString)
            }
            onSourceChanged: reconnect()
        }

        VideoOutput {
            id: video

            anchors.fill: parent
        }
    }

    configContent: ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true
        spacing: 12

        SectionHeader {
            label: "Font Settings"
        }

        LabeledSpinBox {
            bindedProperty: "titleFontSize"
            label: "Title Font Size"
        }

        SectionHeader {
            label: "Stream Settings"
        }

        LabeledTextField {
            bindedProperty: "url"
            label: "URL"
        }
    }
}
