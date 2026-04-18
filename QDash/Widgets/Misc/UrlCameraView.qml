// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

import QtMultimedia

// TODO: rotation, flip, etc
BaseWidget {
    id: widget

    property string url: ""

    propertyKeys: ["url"]

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
            top: titleField.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            margins: 8
        }

        Timer {
            id: connectTimer
            interval: 100
            repeat: false
            onTriggered: player.play()
        }

        MediaPlayer {
            id: player

            source: url

            function reconnect() {
                player.stop()
                connectTimer.start()
            }

            onSourceChanged: reconnect()

            videoOutput: video
            onErrorOccurred: (error, errorString) => {
                                 logs.warn("UrlCameraView",
                                           "Qt reported error " + errorString)
                             }
        }

        VideoOutput {
            id: video
            anchors.fill: parent
        }
    }

    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: Component {
            BaseConfigDialog {
                id: config

                content: ColumnLayout {
                    id: layout
                    spacing: 12
                    anchors.fill: parent
                    anchors.leftMargin: 2
                    clip: true

                    SectionHeader {
                        label: "Font Settings"
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        id: titleFontField

                        label: "Title Font Size"

                        bindedProperty: "titleFontSize"
                    }

                    SectionHeader {
                        label: "Stream Settings"
                    }

                    LabeledTextField {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        label: "URL"

                        bindedProperty: "url"
                    }
                }
            }
        }
    }
}
