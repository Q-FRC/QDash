import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QFRCDashboard

Dialog {
    property url selected

    id: remote

    width: 350
    height: 350

    title: "Remote Layouts"

    background: Rectangle {
        color: Constants.palette.dialogBg

        radius: 12
    }

    standardButtons: Dialog.Ok | Dialog.Close

    anchors.centerIn: Overlay.overlay

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
        height: 50
        width: 50
    }

    ListView {
        id: list
        anchors.fill: parent
        model: rlm

        delegate: RemoteLayout {
            height: 40
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
                anchors.leftMargin: 20

                color: Constants.palette.text
                text: rlm.name(list.currentIndex)
                font.pixelSize: 18

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
