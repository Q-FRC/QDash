// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs as D

import QDash.Constants
import QDash.Dialogs
import QDash.Native.Models

NativeDialog {
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

    onAccepted: {
        selected = rlm.url(list.currentIndex)
        let filename = FileSelect.getSaveFileName(
                qsTr("Save Layout"), StandardPaths.writableLocation(
                    StandardPaths.AppLocalDataLocation),
                "JSON files (*.json);;All files (*)")
        rlm.download(selected, filename)
    }

    onOpened: {
        busy.running = true

        if (rlm.load())
            busy.running = true
        else
            fail.open()
    }

    TextDialog {
        width: 350
        height: 350

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

            onActivated: accept()
            onClicked: list.currentIndex = index
        }
    }
}
