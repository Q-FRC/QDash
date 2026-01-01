// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import QDash.Dialogs
import QDash.Constants
import QDash.Items
import QDash.Native.Logging
import QDash.Native.Helpers

ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height
    visible: true
    title: conn.title

    Material.theme: settings.theme === "light" ? Material.Light : Material.Dark
    Material.accent: accents.qml(
                         settings.accent) // "qml" is the Material Theme accent. Affects checkboxes, etc.

    Material.roundedScale: Material.NotRounded

    property string filename: ""
    property int minWidth: 250
    property int minHeight: 250

    function dsResize() {
        if (settings.resizeToDS) {
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
        // TODO: move to AppLocalData?
        filename = FileSelect.getSaveFileName(
                    qsTr("Save Layout"), StandardPaths.writableLocation(
                        StandardPaths.AppLocalDataLocation),
                    "JSON files (*.json);;All files (*)")

        tlm.save(filename)
    }

    /** LOAD */
    function load() {
        filename = FileSelect.getOpenFileName(
                    qsTr("Open Layout"), StandardPaths.writableLocation(
                        StandardPaths.AppLocalDataLocation),
                    "JSON files (*.json);;All files (*)")

        logs.info("IO", "Loading file " + filename)
        tlm.load(filename)
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

            BetterMenu {
                title: qsTr("&Recent Files...")
                Repeater {
                    model: settings.recentFiles

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
                onTriggered: settingsDialog.openDialog()
            }

            BetterMenu {
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
        }
    }

    /** THE REST */
    Component.onCompleted: {
        Constants.setTheme(settings.theme)
        Constants.setAccent(settings.accent)

        dsResize()
        settings.resizeToDSChanged.connect(dsResize)

        if (settings.loadRecent && settings.recentFiles.length > 0) {
            filename = settings.recentFiles[0]
            if (filename === "" || filename === null)
                return
            tlm.load(filename)
        }
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
    // TODO: Remove leading file:///
    ToolBar {
        id: toolbar

        background: Rectangle {
            color: Constants.accent
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

            color: Constants.palette.text
            font.pixelSize: 16
        }

        Text {
            anchors {
                centerIn: parent
            }

            // TODO: Remove file://
            text: filename === "" ? "No File" : filename

            color: Constants.palette.text
            font.pixelSize: 16
        }
    }

    /* RESIZE ANCHORS */
    Repeater {
        model: [Qt.RightEdge, Qt.LeftEdge, Qt.TopEdge, Qt.BottomEdge, Qt.RightEdge
            | Qt.TopEdge, Qt.RightEdge | Qt.BottomEdge, Qt.LeftEdge
            | Qt.TopEdge, Qt.LeftEdge | Qt.BottomEdge]

        ResizeAnchor {
            required property int modelData
            direction: modelData

            mouseArea.onPressed: window.startSystemResize(direction)
            mouseArea.drag.target: null

            divisor: 80
        }
    }
}
