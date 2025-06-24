import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6

import fields
import items
import config
import basewidget
import constants

import QtMultimedia

// TODO: rotation, flip, etc
PrimitiveWidget {
    id: widget

    suffix: "/streams"

    property string item_url: ""
    property list<string> urlChoices
    property int urlIndex: 0

    property int item_quality: 0
    property int qualityMax: 100

    property int item_fps: 0

    property int item_resW: 0
    property int item_resH: 0

    // onItem_urlChanged: player.resetSource()
    onItem_fpsChanged: player.resetSource()
    onItem_qualityChanged: player.resetSource()
    onItem_resWChanged: player.resetSource()
    onItem_resHChanged: player.resetSource()

    MenuItem {
        id: reconnItem
        text: "Reconnect"
        onTriggered: {
            player.reconnect()
        }
    }

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

    Component.onCompleted: {
        rcMenu.addItem(reconnItem)
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

            source: ""

            function restartVideo() {
                player.play()
            }

            function resetSource() {
                source = Qt.url(item_url + (item_url.includes("?") ? "&" : "?")
                                + (item_quality !== 0 ? "compression=" + item_quality + "&" : "")
                                + (item_fps !== 0 ? "fps=" + item_fps + "&" : "")
                                + (item_resH !== 0
                                   && item_resW !== 0 ? "resolution=" + item_resW + "x"
                                                        + item_resH : ""))
            }

            function reconnect() {
                player.stop()

                connectTimer.start()
            }

            onSourceChanged: {
                reconnect()
            }

            videoOutput: video
            onErrorOccurred: (error, errorString) => {
                                 logs.warn("CameraView",
                                           "Qt reported error " + errorString)

                                 urlIndex++
                                 if (urlIndex >= urlChoices.length) {
                                     urlIndex = 0
                                 }

                                 item_url = urlChoices[urlIndex]

                                 sourceTimer.start()

                                 logs.debug(
                                     "CameraView",
                                     "Cycling to index " + urlIndex + " URL " + item_url)
                             }
        }

        VideoOutput {
            id: video
            anchors.fill: parent
        }
    }

    BaseConfigDialog {
        id: config

        content: ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar
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

                bindedProperty: "item_titleFontSize"
                bindTarget: widget
            }

            SectionHeader {
                label: "Stream Settings"
            }

            LabeledSpinBox {
                Layout.fillWidth: true

                id: fpsField

                label: "FPS"

                bindedProperty: "item_fps"
                bindTarget: widget
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                Text {
                    font.pixelSize: 16 * Constants.scalar
                    text: "Resolution"
                    color: Constants.palette.text
                }

                LabeledSpinBox {
                    Layout.fillWidth: true
                    id: resWField

                    label: "Width"

                    bindedProperty: "item_resW"
                    bindTarget: widget
                }

                Text {
                    font.pixelSize: 18 * Constants.scalar
                    text: "x"
                    color: Constants.palette.text
                }

                LabeledSpinBox {
                    Layout.fillWidth: true
                    id: resHField

                    label: "Height"

                    bindedProperty: "item_resH"
                    bindTarget: widget
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                Text {
                    font.pixelSize: 16 * Constants.scalar
                    text: "Quality"
                    color: Constants.palette.text
                }

                Slider {
                    Layout.fillWidth: true
                    id: qualityField

                    from: 0
                    to: 100
                    stepSize: 10

                    function open() {
                        value = widget.item_quality
                    }

                    function accept() {
                        widget.item_quality = value
                    }
                }
            }

            SectionHeader {
                label: "NT Settings"
            }

            LabeledTextField {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: topicField

                label: "Topic"

                bindedProperty: "item_topic"
                bindTarget: widget
            }
        }
    }
}
