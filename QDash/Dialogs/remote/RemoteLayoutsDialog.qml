// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Native.Models

NativeDialog {
    property url selected

    id: remote

    width: 350
    height: 350

    title: "Remote Layouts"

    standardButtons: Dialog.Ok | Dialog.Close

    onAccepted: {
        selected = rlm.url(list.currentIndex)
        let defaultPath = StandardPaths.writableLocation(
                StandardPaths.AppLocalDataLocation) + "/layout.json"
        let filename = FileSelect.getSaveFileName(
                qsTr("Save Layout"), defaultPath,
                "JSON files (*.json);;All files (*)")
        rlm.download(selected, filename)
    }

    onOpened: {
        busy.running = true

        if (rlm.load())
            busy.running = true
    }

    MessageDialog {
        width: 350
        height: 350

        id: fail
        standardButtons: Dialog.Ok
        title: "Error"
        text: "You must be connected to a robot to download remote layouts."

        onClosed: remote.close()
    }

    RemoteLayoutModel {
        id: rlm

        onFileOpened: filename => {
                          window.filename = filename
                          TabListModel.load(filename)
                          remote.close()
                      }

        onListReady: {
            busy.running = false
        }

        onFailed: {
            fail.open()
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
        anchors.margins: 20
        model: rlm

        delegate: RemoteLayout {
            height: 40
            width: ListView.view.width

            onActivated: accept()
            onClicked: list.currentIndex = index
        }
    }
}
