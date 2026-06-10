// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Backend.Models
import QtCore
import QtQuick
import QtQuick.Controls

Loader {
    id: loader

    property Component src: Component {
        CarboxylDialog {
            id: remote

            property url selected

            implicitHeight: 350
            implicitWidth: 350
            popupType: Popup.Window
            standardButtons: Dialog.Ok | Dialog.Close
            title: "Remote Layouts"

            onAccepted: {
                selected = RemoteLayoutModel.url(list.currentIndex)
                let defaultPath = StandardPaths.writableLocation(StandardPaths.AppLocalDataLocation) + "/layout.json"
                let filename = CarboxylQuickInterface.getSaveFileName(qsTr("Save Layout"), defaultPath, "JSON files (*.json);;All files (*)")
                RemoteLayoutModel.download(selected, filename)
            }
            onClosed: loader.active = false
            onOpened: {
                busy.running = true

                if (RemoteLayoutModel.load())
                    busy.running = true
            }

            CarboxylMessageDialog {
                id: fail

                height: 350
                standardButtons: Dialog.Ok
                text: "You must be connected to a robot with a web server to download remote layouts."
                title: "Error"
                width: 350

                onClosed: remote.close()
            }

            Connections {
                function onFailed() {
                    fail.open()
                }

                function onFileOpened(filename) {
                    window.filename = filename
                    TabListModel.load(filename)
                    remote.close()
                }

                function onListReady() {
                    busy.running = false
                }

                target: RemoteLayoutModel
            }

            BusyIndicator {
                id: busy

                anchors.centerIn: parent
                height: 50
                running: false
                width: 50
            }

            ListView {
                id: list

                anchors.fill: parent
                anchors.margins: 20
                model: RemoteLayoutModel

                delegate: RemoteLayout {
                    height: 40
                    width: ListView.view.width

                    onActivated: accept()
                    onClicked: list.currentIndex = index
                }
            }
        }
    }

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: item.open()
}
