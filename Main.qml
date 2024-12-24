import QtQuick
import QtQuick.Controls

import QFRCDashboard

ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height
    visible: true
    title: titleManager.title

    AccentEditor {
        id: accentEditor

        anchors.centerIn: Overlay.overlay
    }

    AboutDialog {
        id: about
    }

    Shortcut {
        sequence: "Ctrl+,"
        onActivated: screen.settingsDialog()
    }

    menuBar: MenuBar {
        contentWidth: parent.width

        MenuBarItem {
            text: qsTr("&Settings")
            onTriggered: screen.settingsDialog()
        }

        Menu {
            contentWidth: 175
            title: qsTr("&File")
            Action {
                text: qsTr("&Save")
                onTriggered: screen.save()
                shortcut: "Ctrl+S"
            }
            Action {
                text: qsTr("Save &As")
                onTriggered: screen.saveAsAction()
                shortcut: "Ctrl+Shift+S"
            }
            Action {
                text: qsTr("&Open...")
                onTriggered: screen.loadAction()
                shortcut: "Ctrl+O"
            }
            Menu {
                title: qsTr("&Recent Files...")
                Repeater {
                    model: settings.recentFiles

                    delegate: MenuItem {
                        text: qsTr("&" + index + ". " + modelData)
                        onTriggered: {
                            if (modelData === "" || modelData === null) return;
                            tlm.clear()
                            tlm.load(modelData)
                        }
                    }
                }
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


