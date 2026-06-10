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
PrimitiveWidget {
    id: widget

    property int item_fps: 0
    property int item_quality: 0
    property int item_resH: 0
    property int item_resW: 0
    property string item_url: ""
    property int qualityMax: 100
    property list<string> urlChoices
    property int urlIndex: 0

    function fixUrls(value) {
        for (var i = 0; i < value.length; ++i) {
            if (value[i].startsWith("mjpg:"))
                value[i] = value[i].substring(5)
        }
    }

    function update(value) {
        urlChoices = value
        fixUrls(urlChoices)

        urlIndex = 0

        if (urlChoices.length > 0)
            item_url = urlChoices[0]

        player.resetSource()

        sourceTimer.start()
    }

    // TODO(crueter): Fix
    propertyKeys: ["item_url", "item_fps", "item_quality", "item_resW", "item_resH"]
    suffix: "/streams"

    configContent: ColumnLayout {
        id: layout

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

        LabeledSpinBox {
            bindedProperty: "item_fps"
            label: "FPS"
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                font.pixelSize: 16
                text: "Resolution"
            }

            LabeledSpinBox {
                bindedProperty: "item_resW"
                label: "Width"
            }

            Label {
                font.pixelSize: 18
                text: "x"
            }

            LabeledSpinBox {
                bindedProperty: "item_resH"
                label: "Height"
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            Label {
                font.pixelSize: 16
                text: "Quality"
            }

            Slider {
                function accept() {
                    widget.item_quality = value
                }

                function open() {
                    value = widget.item_quality
                }

                from: 0
                stepSize: 10
                to: 100
            }
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
        }
    }
    menuExtension: Component {
        MenuItem {
            id: reconnItem

            text: "Reconnect"

            onTriggered: player.reconnect()
        }
    }

    // onItem_urlChanged: player.resetSource()
    onItem_fpsChanged: player.resetSource()
    onItem_qualityChanged: player.resetSource()
    onItem_resHChanged: player.resetSource()
    onItem_resWChanged: player.resetSource()

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

            onTriggered: player.restartVideo()
        }

        Timer {
            id: sourceTimer

            interval: 200
            repeat: false

            onTriggered: {
                player.source = ""
                player.resetSource()
            }
        }

        MediaPlayer {
            id: player

            function reconnect() {
                player.stop()

                connectTimer.start()
            }

            function resetSource() {
                source = Qt.url(item_url + (item_url.includes("?") ? "&" : "?") + (item_quality !== 0 ? "compression=" + item_quality + "&" : "") + (item_fps !== 0 ? "fps=" + item_fps + "&" : "") + (item_resH !== 0 && item_resW !== 0 ? "resolution=" + item_resW + "x" + item_resH : ""))
            }

            function restartVideo() {
                player.play()
            }

            source: ""
            videoOutput: video

            onErrorOccurred: (error, errorString) => {
                logs.warn("CameraView", "Qt reported error " + errorString)

                urlIndex++
                if (urlIndex >= urlChoices.length) {
                    urlIndex = 0
                }

                item_url = urlChoices[urlIndex]

                sourceTimer.start()

                logs.debug("CameraView", "Cycling to index " + urlIndex + " URL " + item_url)
            }
            onSourceChanged: {
                reconnect()
            }
        }

        VideoOutput {
            id: video

            anchors.fill: parent
        }
    }
}
