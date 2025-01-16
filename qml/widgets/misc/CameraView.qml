import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6

import QtMultimedia

import QFRCDashboard

BaseWidget {
    id: widget
    property string item_topic

    property var item_url: ""
    property list<string> urlChoices

    property int item_quality: 0
    property int qualityMax: 100

    property int item_fps: 0
    property int fpsMax: 240

    property int item_resW: 0
    property int item_resH: 0

    onItem_urlChanged: player.resetSource()
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

            if (value[i].includes("local")) {
                value.splice(i, 1)
                --i
            }
        }
    }

    function updateTopic(ntTopic, ntValue) {
        if (ntTopic === item_topic + "/streams") {
            urlChoices = ntValue
            fixUrls(urlChoices)

            if (urlChoices.length > 0 && item_url === "")
                item_url = urlChoices[0]

            player.resetSource()
        }
    }

    Component.onCompleted: {
        rcMenu.addItem(reconnItem)

        topicStore.topicUpdate.connect(updateTopic)

        item_topic = model.topic
    }

    Component.onDestruction: {
        if (topicStore !== null) {
            topicStore.topicUpdate.disconnect(updateTopic)
            topicStore.unsubscribe(item_topic)
        }
    }

    Rectangle {
        color: "transparent"
        anchors {
            top: titleField.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            margins: 8
        }

        Timer {
            id: timer
        }

        MediaPlayer {
            id: player

            source: ""

            function restartVideo() {
                player.play()
            }

            function resetSource() {
                source = Qt.url(item_url + (item_quality !== 0 ? "compression="
                                                                 + item_quality + "&" : "")
                                + (item_fps !== 0 ? "fps=" + item_fps + "&" : "")
                                + (item_resH !== Qt.size(
                                       0, 0) ? "resolution=" + item_resW + "x" + item_resH : ""))
            }

            function reconnect() {
                player.stop()
                timer.interval = 100
                timer.repeat = false
                timer.onTriggered.connect(restartVideo)
                timer.start()
            }

            onSourceChanged: {
                reconnect()
            }

            videoOutput: video
            onErrorOccurred: (error, errorString) => console.error(
                                 "CameraView: error:", errorString)
        }

        VideoOutput {
            id: video
            anchors.fill: parent
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic + "/streams")
        topicStore.subscribe(item_topic + "/streams")
        model.topic = item_topic

        updateTopic(item_topic + "/streams",
                    topicStore.getValue(model.topic + "/streams"))
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fpsField.open()
            resHField.open()
            resWField.open()
            qualityField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fpsField.accept()
            resHField.accept()
            resWField.accept()
            qualityField.accept()
        }

        ScrollView {
            contentWidth: width - 5 * Constants.scalar - effectiveScrollBarWidth

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: -20

                rightMargin: 5 * Constants.scalar
            }

            ColumnLayout {
                id: layout
                spacing: 25 * Constants.scalar

                anchors.fill: parent
                anchors.leftMargin: 2

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

                    // Layout.preferredWidth: 300
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
}
