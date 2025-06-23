import QtCore
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs as D

import QDash

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

    onOpened: {
        busy.running = true

        if (rlm.load()) {
            busy.running = true
        } else {
            console.log("FAIL")
            fail.open()
        }
    }

    TextDialog {
        width: 350 * Constants.scalar
        height: 350 * Constants.scalar

        modal: Qt.WindowModal

        id: fail
        standardButtons: "Ok"
        title: "Error"
        text: "You must be connected to a robot to download remote layouts."

        onClosed: remote.close()
    }

    RemoteLayoutModel {
        id: rlm

        onFileOpened: filename => {
                          tlm.load(filename)
                          remote.close()
                      }
        onListReady: {
            busy.running = false
        }
    }

    D.FileDialog {
        id: saveDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: D.FileDialog.SaveFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: rlm.download(selected, selectedFile)
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
    }
}
