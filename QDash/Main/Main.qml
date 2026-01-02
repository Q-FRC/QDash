// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import QDash.Dialogs
import Carboxyl.Clover
import QDash.Items

import QDash.Native.Logging
import QDash.Native.Helpers

import Carboxyl.Clover

ApplicationWindow {
    id: window
    visible: true
    title: conn.title

    palette: Clover.theme

    width: QDashSettings.windowWidth
    height: QDashSettings.windowHeight

    x: QDashSettings.windowX === -1 ? null : QDashSettings.windowX
    y: QDashSettings.windowY === -1 ? null : QDashSettings.windowY

    property string filename: ""
    property int minWidth: 250
    property int minHeight: 250

    function dsResize() {
        if (QDashSettings.resizeToDS) {
            logs.debug("UI", "DS Resize")

            height = platformHelper.screenHeight() - 236
            width = platformHelper.screenWidth()

            x = 0
            y = 0
        }
    }

    // Dialogs
    Loader {
        id: remoteLayouts

        active: CompileDefinitions.useNetwork
        sourceComponent: active ? Qt.createComponent(
                                      "../Dialogs/remote/RemoteLayoutsDialog.qml") : null
    }

    AboutDialog {
        id: about
    }

    NotificationPopup {
        id: notif

        x: parent.width - width - 10
        y: parent.height - height - 10
    }

    /** SAVE */
    function save() {
        if (filename === "")
            return saveAs()

        tlm.save(filename)
    }

    function saveAs() {
        console.log(QDashApplication.dataLocation + "/layout.json")
        let newName = FileSelect.getSaveFileName(
                qsTr("Save Layout"),
                QDashApplication.dataLocation + "/layout.json",
                "JSON files (*.json);;All files (*)")

        if (newName !== "") {
            filename = newName
            logs.info("IO", "Saving to " + filename)
            tlm.save(filename)
        }
    }

    /** LOAD */
    function load() {
        let newName = FileSelect.getOpenFileName(
                qsTr("Open Layout"),
                QDashApplication.dataLocation + "/layout.json",
                "JSON files (*.json);;All files (*)")

        if (newName !== "") {
            fileName = newName
            logs.info("IO", "Loading file " + filename)
            tlm.load(filename)
        }
    }

    /** SERVER SETTINGS */
    SettingsDialog {
        id: settingsDialog
    }

    /** MENU BAR */
    menuBar: MenuBar {
        Menu {
            z: 25
            title: qsTr("&File")

            Action {
                text: qsTr("&Save")
                onTriggered: save()
                shortcut: "Ctrl+S"
            }

            Action {
                text: qsTr("Save &As")
                onTriggered: saveAs()
                shortcut: "Ctrl+Shift+S"
            }

            Action {
                text: qsTr("&Open...")
                onTriggered: load()
                shortcut: "Ctrl+O"
            }

            Menu {
                title: qsTr("&Recent Files...")
                Repeater {
                    model: QDashSettings.recentFiles

                    delegate: MenuItem {
                        text: qsTr("&" + index + ". " + platformHelper.baseName(
                                       modelData))
                        onTriggered: {
                            if (modelData === "" || modelData === null)
                                return
                            tlm.clear()
                            tlm.load(modelData)
                        }
                    }
                }
            }
            Action {
                enabled: CompileDefinitions.useNetwork
                text: qsTr("Remote &Layouts...")
                onTriggered: remoteLayouts.item.open()

                shortcut: "Ctrl+L"
            }
        }

        Menu {
            z: 25
            title: qsTr("&Edit")

            Action {
                text: qsTr("&Paste Widget")
                onTriggered: screen.paste()
                shortcut: "Ctrl+V"
            }

            Action {
                text: qsTr("&Settings")
                shortcut: "Ctrl+,"
                onTriggered: settingsDialog.open()
            }

            Menu {
                title: qsTr("&Extra Widgets")

                Repeater {
                    model: CompileDefinitions.extraWidgets

                    delegate: MenuItem {
                        text: qsTr("&" + model.key)
                        onTriggered: {
                            // TODO: make extensible
                            screen.addWidget(model.key, "", model.value)
                        }
                    }
                }
            }
        }

        Menu {
            z: 25
            title: qsTr("&Tab")
            Action {
                text: qsTr("&New Tab")
                onTriggered: screen.newTab()
                shortcut: "Ctrl+T"
            }

            Action {
                text: qsTr("&Close Tab")
                onTriggered: screen.closeTab()
                shortcut: "Ctrl+W"
            }

            Action {
                text: qsTr("Configu&re Tab")
                onTriggered: screen.configTab()
                shortcut: "Ctrl+R"
            }
        }

        Menu {
            z: 25
            title: qsTr("&About")
            Action {
                text: qsTr("&About QDash")
                onTriggered: about.open()
            }

            Action {
                text: qsTr("About &Qt")
                onTriggered: QDashApplication.aboutQt()
            }
        }
    }

    /** THE REST */
    Component.onCompleted: {
        Clover.accent = Clover.accents[QDashSettings.accent]
        Clover.theme = Clover.themes[QDashSettings.theme]

        dsResize()
        QDashSettings.resizeToDSChanged.connect(dsResize)

        if (QDashSettings.loadRecent && QDashSettings.recentFiles.length > 0) {
            filename = QDashSettings.recentFiles[0]
            if (filename === "" || filename === null)
                return
            tlm.load(filename)
        }
    }

    Component.onDestruction: {
        QDashSettings.windowWidth = width
        QDashSettings.windowHeight = height
        QDashSettings.windowY = y
        QDashSettings.windowX = x
    }

    MainScreen {
        id: screen
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: toolbar.top
        }
    }

    /** Status Bar */
    ToolBar {
        id: toolbar

        background: Rectangle {
            color: Clover.theme.highlight
        }

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        implicitHeight: 30
        Text {
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter

                leftMargin: 20
            }

            text: "Status: " + conn.status

            color: Clover.theme.text
            font.pixelSize: 16
        }

        Text {
            anchors {
                centerIn: parent
            }

            text: filename === "" ? "No File" : filename

            color: Clover.theme.text
            font.pixelSize: 16
        }
    }
}
