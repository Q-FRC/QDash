import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import QtQuick.Controls.Universal

import QFRCDashboard

ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height
    visible: true
    title: titleManager.title

    Universal.theme: settings.theme === "light" ? Universal.Light : Universal.Dark
    Universal.accent: accents.qml(settings.accent) // "qml" is the Universal Theme accent. Affects checkboxes, etc.

    property string filename: ""

    AccentEditor {
        id: accentEditor

        anchors.centerIn: Overlay.overlay
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
    FileDialog {
        id: saveDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.SaveFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: saveAs();
    }

    function save() {
        if (filename === "")
            return saveDialog.open()

        tlm.save(filename)
    }

    function saveAs() {
        filename = saveDialog.selectedFile

        tlm.save(filename)
    }

    /** LOAD */
    FileDialog {
        id: loadDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.OpenFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: load()
    }

    function load() {
        filename = loadDialog.selectedFile
        tlm.load(filename)
    }

    /** SERVER SETTINGS */
    SettingsDialog {
        id: settingsDialog
    }

    /** REMOTE LAYOUTS */
    MessageDialog {
        id: fail
        buttons: MessageDialog.Ok
        title: "Error"
        text: "You must be connected to a robot to download remote layouts.";

        modality: Qt.WindowModal
    }

    RemoteLayoutsDialog {
        id: remoteLayouts
    }

    /** THE REST */

    Component.onCompleted: {
        Constants.setTheme(settings.theme)
        Constants.setAccent(settings.accent)

        if (settings.loadRecent && settings.recentFiles.length > 0) {
            filename = settings.recentFiles[0]
            if (filename === "" || filename === null) return;
            tlm.load(filename)
        }
    }

    Shortcut {
        sequence: "Ctrl+,"
        onActivated: settingsDialog.openDialog()
    }

    menuBar: MenuBar {
        contentWidth: parent.width

        MenuBarItem {
            text: qsTr("&Settings")
            onTriggered: settingsDialog.openDialog()
        }

        Menu {
            contentWidth: 175
            title: qsTr("&File")
            Action {
                text: qsTr("&Save")
                onTriggered: save()
                shortcut: "Ctrl+S"
            }
            Action {
                text: qsTr("Save &As")
                onTriggered: saveDialog.open()
                shortcut: "Ctrl+Shift+S"
            }
            Action {
                text: qsTr("&Open...")
                onTriggered: loadDialog.open()
                shortcut: "Ctrl+O"
            }
            Menu {
                title: qsTr("&Recent Files...")
                Repeater {
                    model: settings.recentFiles

                    delegate: MenuItem {
                        text: qsTr("&" + index + ". " + platformHelper.baseName(modelData))
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
                text: qsTr("Remote &Layouts...")
                onTriggered: remoteLayouts.openDialog();
                shortcut: "Ctrl+L"
            }
        }

        Menu {
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
            title: qsTr("&Widget")
            Action {
                text: qsTr("&Paste")
                onTriggered: screen.paste()
                shortcut: "Ctrl+V"
            }
        }

        MenuBarItem {
            text: qsTr("&About")
            onTriggered: about.open()
        }
    }

    MainScreen {
        id: screen
        anchors.fill: parent
    }
}
