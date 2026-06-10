// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour
import QDash.Components

import QDash.Dialogs
import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window

    property string filename: ""
    property int minHeight: 250
    property int minWidth: 250

    function dsResize() {
        if (QDashSettings.resizeToDS) {
            logs.debug("UI", "DS Resize")

            QDashSettings.windowHeight = PlatformHelper.screenHeight() - 200 - PlatformHelper.titlebarHeight(this)
            QDashSettings.windowWidth = PlatformHelper.screenWidth();

            // Explicitly disallow resizing, because the user probably doesn't want to
            // accidentally overwrite the height. Note that some Linux window managers (and Wayland)
            // will just straight up ignore this.
            maximumHeight = height
            minimumHeight = height

            QDashSettings.windowX = 0
            QDashSettings.windowY = PlatformHelper.titlebarHeight(this)
        }
    }

    /** LOAD */
    function load() {
        let newName = CarboxylQuickInterface.getOpenFileName(qsTr("Open Layout"), QDashApplication.dataLocation + "/layout.json", "JSON files (*.json);;All files (*)")

        if (newName !== "") {
            filename = newName
            logs.info("IO", "Loading file " + filename)
            TabListModel.load(filename)
        }
    }

    /** SAVE */
    function save() {
        if (filename === "")
            return saveAs()

        TabListModel.save(filename)
    }

    function saveAs() {
        let newName = CarboxylQuickInterface.getSaveFileName(qsTr("Save Layout"), QDashApplication.dataLocation + "/layout.json", "JSON files (*.json);;All files (*)")

        if (newName !== "") {
            filename = newName
            logs.info("IO", "Saving to " + filename)
            TabListModel.save(filename)
        }
    }

    height: QDashSettings.windowHeight
    width: QDashSettings.windowWidth

    x: QDashSettings.windowX === -1 ? 0 : QDashSettings.windowX
    y: QDashSettings.windowY === -1 ? 0 : QDashSettings.windowY

    palette: Clover.theme
    title: ConnManager.title
    visible: true

    /** MENU BAR */
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            z: 25

            Action {
                shortcut: "Ctrl+S"
                text: qsTr("&Save")

                onTriggered: save()
            }

            Action {
                shortcut: "Ctrl+Shift+S"
                text: qsTr("Save &As")

                onTriggered: saveAs()
            }

            Action {
                shortcut: "Ctrl+O"
                text: qsTr("&Open...")

                onTriggered: load()
            }

            Menu {
                title: qsTr("&Recent Files...")

                Repeater {
                    model: QDashSettings.recentFiles

                    delegate: MenuItem {
                        text: qsTr("&" + index + ". " + PlatformHelper.baseName(modelData))

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
                shortcut: "Ctrl+L"
                text: qsTr("Remote &Layouts...")

                onTriggered: remoteLayouts.open()
            }
        }

        Menu {
            title: qsTr("&Edit")
            z: 25

            Action {
                shortcut: "Ctrl+V"
                text: qsTr("&Paste Widget")

                onTriggered: screen.paste()
            }

            Action {
                shortcut: "Ctrl+,"
                text: qsTr("&Settings")

                onTriggered: settingsDialog.open()
            }

            Menu {
                enabled: CompileDefinitions.extraWidgets.count > 0
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
            title: qsTr("&Tab")
            z: 25

            Action {
                shortcut: "Ctrl+T"
                text: qsTr("&New Tab")

                onTriggered: screen.newTab()
            }

            Action {
                shortcut: "Ctrl+W"
                text: qsTr("&Close Tab")

                onTriggered: screen.closeTab()
            }

            // FUN FACT: Configure is a keyword that gets hijacked by macOS's absolutely horrifically garbage
            // menu implementation. AWESOME! GREAT! WONDERFUL!
            Action {
                shortcut: "Ctrl+E"
                text: qsTr("&Edit Tab")

                onTriggered: screen.configTab()
            }
        }

        Menu {
            title: qsTr("&About")
            z: 25

            Action {
                text: qsTr("&About QDash")

                onTriggered: about.active = true
            }

            Action {
                text: qsTr("About &Qt")

                onTriggered: CarboxylApplication.aboutQt()
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
        } else if (QDashSettings.loadRecent && QDashSettings.recentFiles.length > 0) {
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

    // Warn the user if they close before saving.
    onClosing: close => {
        if (TabListModel.modified) {
            let button = CarboxylQuickInterface.showMessageBox(CarboxylEnums.Warning, "Save Changes?", "Your changes will be lost if you don't save.", Dialog.Yes | Dialog.No | Dialog.Cancel)

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

    Connections {
        function onModifiedChanged() {
            ConnManager.modified = TabListModel.modified
        }

        target: TabListModel
    }

    /* DIALOGS */
    RemoteLayoutsDialog {
        id: remoteLayouts
    }

    // TODO(crueter): Investigate additional RAM stuck in memory after Loaders are freed
    AboutDialog {
        id: about
    }

    StyleWarnDialog {
        id: styleWarnDialog
    }

    // TODO(crueter): Seems like making a LazyLoadDialog in Carboxyl land could be possible.
    SettingsDialog {
        id: settingsDialog
    }

    NotificationPopup {
        id: notif

        x: parent.width - width - 10
        y: parent.height - toolbar.height - height - 10
    }

    MainScreen {
        id: screen

        anchors {
            bottom: toolbar.top
            left: parent.left
            right: parent.right
            top: parent.top
        }
    }

    /** Status Bar */
    ToolBar {
        id: toolbar

        implicitHeight: 30

        background: Rectangle {
            color: Clover.theme.highlight
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        Label {
            font.pixelSize: 16
            text: ConnManager.status

            anchors {
                left: parent.left
                leftMargin: 20
                verticalCenter: parent.verticalCenter
            }
        }

        Label {
            font.pixelSize: 16
            text: filename === "" ? "No File" : PlatformHelper.filename(filename)

            anchors {
                centerIn: parent
            }
        }
    }
}
