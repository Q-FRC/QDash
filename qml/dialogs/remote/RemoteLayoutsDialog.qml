import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QFRCDashboard

AnimatedDialog {
    property url selected

    id: remote

    width: 350 * Constants.scalar
    height: 350 * Constants.scalar

    title: "Remote Layouts"

    background: Rectangle {
        color: Constants.palette.dialogBg

        radius: 12
    }

    standardButtons: Dialog.Ok | Dialog.Close

    onAccepted: {
        selected = rlm.url(list.currentIndex)
        saveDialog.open()
    }

    RemoteLayoutModel {
        id: rlm

        onFileOpened: (filename) => {
                          tlm.load(filename)
                          remote.close()
                      }
        onListReady: {
            busy.running = false
        }
    }

    FileDialog {
        id: saveDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.SaveFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: rlm.download(selected, selectedFile);
    }

    function openDialog() {
        if (rlm.load()) {
            busy.running = true
        } else {
            fail.open()
            return
        }

        open()
    }

    BusyIndicator {
        id: busy

        running: false
        anchors.centerIn: parent
        height: 50 * Constants.scalar
        width: 50 * Constants.scalar
    }

    ListView {
        id: list
        anchors.fill: parent
        model: rlm

        delegate: RemoteLayout {
            height: 40 * Constants.scalar
            width: parent.width

            onActivated: {
                accept()
            }

            onClicked: {
                list.currentIndex = index
            }
        }

        highlight: Rectangle {
            z: 1
            color: '#00aaff'
            radius: 5
            border {
                color: Constants.palette.text
                width: 2
            }

            Text {
                anchors.fill: parent
                anchors.leftMargin: 20 * Constants.scalar

                color: Constants.palette.text
                text: rlm.name(list.currentIndex)
                font.pixelSize: Math.round(18 * Constants.scalar)

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
