import QtQuick 2.15
import QtQuick.Controls 2.15

import QtMultimedia

import QFRCDashboard

BaseWidget {
    property string item_host: "127.0.0.1"

    property int item_port: 1181
    property int portMax: 65535

    property int item_quality: 0
    property int qualityMax: 100

    property int item_fps: 0
    property int fpsMax: 240

    property size item_resolution: Qt.size(0, 0)

    onItem_fpsChanged: player.resetSource()
    onItem_portChanged: player.resetSource()
    onItem_qualityChanged: player.resetSource()
    onItem_resolutionChanged: player.resetSource()
    onItem_hostChanged: player.resetSource()

    MenuItem {
        id: reconnItem
        text: "Reconnect"
        onTriggered: {
            player.reconnect()
        }
    }

    Component.onCompleted: {
        rcMenu.addItem(reconnItem)
    }

    Rectangle {
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
                console.log("Restarting.")
                player.play()
            }

            function resetSource() {
                source = Qt.url(item_host + ":" + item_port + "/stream.mjpg?" + (item_quality !== 0 ? "compression=" + item_quality + "&" : "") + (item_fps !== 0 ? "fps=" + item_fps + "&" : "") + (item_resolution !== Qt.size(0, 0) ? "resolution=" + item_resolution.width + "x" + item_resolution.height : ""))
            }

            function reconnect() {
                player.stop()
                timer.interval = 100
                timer.repeat = false
                timer.onTriggered.connect(restartVideo)
                timer.start()
            }

            onSourceChanged: {
                console.log(source)
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
}
