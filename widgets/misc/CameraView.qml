import QtQuick 2.15
import QtQuick.Controls 2.15

import QtMultimedia

import QFRCDashboard

BaseWidget {
    property string item_topic

    property var item_url: ""
    property list<string> urlChoices

    property int item_quality: 0
    property int qualityMax: 100

    property int item_fps: 0
    property int fpsMax: 240

    property size item_resolution: Qt.size(0, 0)

    onItem_urlChanged: player.resetSource()
    onItem_fpsChanged: player.resetSource()
    onItem_qualityChanged: player.resetSource()
    onItem_resolutionChanged: player.resetSource()

    MenuItem {
        id: reconnItem
        text: "Reconnect"
        onTriggered: {
            player.reconnect()
        }
    }

    function fixUrls(value) {
        for (let i = 0; i < value.length; ++i) {
            if (value[i].startsWith("mjpg:")) value[i] = value[i].substring(5)
        }
    }

    function updateTopic(ntTopic, ntValue) {
        if (ntTopic === item_topic + "/streams") {
            urlChoices = ntValue
            fixUrls(urlChoices)

            if (urlChoices.length > 0 && item_url === "") item_url = urlChoices[0]
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
                source = Qt.url(item_url + (item_quality !== 0 ? "compression=" + item_quality + "&" : "") + (item_fps !== 0 ? "fps=" + item_fps + "&" : "") + (item_resolution !== Qt.size(0, 0) ? "resolution=" + item_resolution.width + "x" + item_resolution.height : ""))
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

        updateTopic(item_topic + "/streams", topicStore.getValue(model.topic + "/streams"))
    }
}
