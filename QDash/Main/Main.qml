// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Dialogs
import QDash.Items

import QDash.Native.Logging
import QDash.Native.Helpers

ApplicationWindow {
    id: window
    visible: true
    title: conn.title

    palette: Clover.theme

    width: QDashSettings.windowWidth
    height: QDashSettings.windowHeight

    x: QDashSettings.windowX === -1 ? 0 : QDashSettings.windowX
    y: QDashSettings.windowY === -1 ? 0 : QDashSettings.windowY

    property string filename: ""
    property int minWidth: 250
    property int minHeight: 250

    function dsResize() {
        if (QDashSettings.resizeToDS) {
            logs.debug("UI", "DS Resize")

            // TODO(crueter): Better heuristic for DS resize.
            QDashSettings.windowHeight = PlatformHelper.screenHeight() - 230
            QDashSettings.windowWidth = PlatformHelper.screenWidth()

            // Explicitly disallow resizing, because the user probably doesn't want to
            // accidentally overwrite the height. Note that some Linux window managers (and Wayland)
            // will just straight up ignore this.
            maximumHeight = height
            minimumHeight = height

            QDashSettings.windowX = 0
            QDashSettings.windowY = PlatformHelper.titlebarHeight(this)
        }
    }

    Connections {
        target: TabListModel

        function onModifiedChanged() {
            conn.modified = TabListModel.modified
        }
    }

    /* DIALOGS */
    Loader {
        id: remoteLayouts

        active: false
        sourceComponent: active ? Qt.createComponent(
                                      "../Dialogs/remote/RemoteLayoutsDialog.qml") : null

        onLoaded: item.open()
    }

    Loader {
        id: about
        active: false
        asynchronous: true
        onLoaded: item.open()

        sourceComponent: Component {
            AboutDialog {
                onClosed: settingsDialog.active = false
            }
        }
    }

    Loader {
        id: styleWarnDialog
        active: false
        asynchronous: true
        onLoaded: item.open()

        function open() {
            active = true
        }

        sourceComponent: Component {
            MessageDialog {
                text: qsTr("To apply the new style, QDash will now close and re-open.")
                icon: CarboxylEnums.Warning
                title: qsTr("Reloading")
                standardButtons: DialogButtonBox.Ok

                onClosed: QDashApplication.reload()
            }
        }
    }

    // TODO(crueter): Seems like making a LazyLoadDialog in Carboxyl land could be possible.
    Loader {
        id: settingsDialog
        active: false
        asynchronous: true
        onLoaded: item.open()

        sourceComponent: Component {
            SettingsDialog {
                onClosed: settingsDialog.active = false
            }
        }
    }

    // Warn the user if they close before saving.
    onClosing: close => {
                   if (TabListModel.modified) {
                       let button = CarboxylQuickInterface.showMessageBox(
                           CarboxylEnums.Warning, "Save Changes?",
                           "Your changes will be lost if you don't save.",
                           Dialog.Yes | Dialog.No | Dialog.Cancel)

                       switch (button) {
                           case Dialog.Yes:
                           save()
                           break
                           case Dialog.No:
                           break
                           case Dialog.Cancel:
                           close.accepted = false
                           break
                       }
                   }
               }

    NotificationPopup {
        id: notif

        x: parent.width - width - 10
        y: parent.height - toolbar.height - height - 10
    }

    /** SAVE */
    function save() {
        if (filename === "")
            return saveAs()

        TabListModel.save(filename)
    }

    function saveAs() {
        let newName = FileSelect.getSaveFileName(
                qsTr("Save Layout"),
                QDashApplication.dataLocation + "/layout.json",
                "JSON files (*.json);;All files (*)")

        if (newName !== "") {
            filename = newName
            logs.info("IO", "Saving to " + filename)
            TabListModel.save(filename)
        }
    }

    /** LOAD */
    function load() {
        let newName = FileSelect.getOpenFileName(
                qsTr("Open Layout"),
                QDashApplication.dataLocation + "/layout.json",
                "JSON files (*.json);;All files (*)")

        if (newName !== "") {
            filename = newName
            logs.info("IO", "Loading file " + filename)
            TabListModel.load(filename)
        }
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
                        text: qsTr("&" + index + ". " + PlatformHelper.baseName(
                                       modelData))
                        onTriggered: {
                            if (modelData === "" || modelData === null)
                                return
                            TabListModel.clear()
                            filename = modelData
                            TabListModel.load(modelData)
                        }
                    }
                }
            }

            Action {
                enabled: CompileDefinitions.useNetwork
                text: qsTr("Remote &Layouts...")
                onTriggered: remoteLayouts.active = true

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
                onTriggered: settingsDialog.active = true
            }

            Menu {
                title: qsTr("&Extra Widgets")
                enabled: CompileDefinitions.extraWidgets.count > 0

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

            // FUN FACT: Configure is a keyword that gets hijacked by macOS's absolutely horrifically garbage
            // menu implementation. AWESOME! GREAT! WONDERFUL!
            Action {
                text: qsTr("&Edit Tab")
                onTriggered: screen.configTab()
                shortcut: "Ctrl+E"
            }
        }

        Menu {
            z: 25
            title: qsTr("&About")
            Action {
                text: qsTr("&About QDash")
                onTriggered: about.active = true
            }

            Action {
                text: qsTr("About &Qt")
                onTriggered: QDashApplication.aboutQt()
            }

            Action {
                text: qsTr("About &Carboxyl")
                onTriggered: CarboxylApplication.aboutCarboxyl()
            }
        }
    }

    /** THE REST */
    Component.onCompleted: {
        Clover.registerTheme(CustomThemes.hannahDark)

        Clover.accent = Clover.accents[QDashSettings.accent]
        Clover.theme = Clover.themes[QDashSettings.theme]

        dsResize()
        QDashSettings.resizeToDSChanged.connect(dsResize)

        if (CompileDefinitions.singleFile) {
            load()
        } else if (QDashSettings.loadRecent
                   && QDashSettings.recentFiles.length > 0) {
            filename = QDashSettings.recentFiles[0]
            if (filename === "" || filename === null)
                return
            TabListModel.load(filename)
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

        Label {
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter

                leftMargin: 20
            }

            text: conn.status

            font.pixelSize: 16
        }

        Label {
            anchors {
                centerIn: parent
            }

            text: filename === "" ? "No File" : PlatformHelper.filename(
                                        filename)

            font.pixelSize: 16
        }
    }
}
