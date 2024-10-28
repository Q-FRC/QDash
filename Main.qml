import QtQuick
import QtQuick.Controls

import QFRCDashboard

ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height
    visible: true
    title: titleManager.title

    menuBar: MenuBar {
        contentWidth: parent.width

        Menu {
            contentWidth: 210
            title: qsTr("&Settings")
            Action {
                text: qsTr("&Server Settings...")
                onTriggered: screen.serverSettings()
                shortcut: "Ctrl+E"
            }
            MenuItem {
                text: "&Load Most Recent File?"
                checkable: true
                checked: settings.loadRecent
                onCheckedChanged: settings.loadRecent = checked
            }

            Menu {
                title: qsTr("&Theme")
                Repeater {
                    model: ["Light", "Dark", "Midnight"]

                    MenuItem {
                        text: "&" + modelData
                        checkable: true
                        checked: settings.theme === modelData.toLowerCase()
                        onCheckedChanged: {
                            if (checked)
                                Constants.setTheme(modelData.toLowerCase())
                        }
                    }
                }
            }

            Menu {
                title: qsTr("&Accent")

                Repeater {
                    model: accents

                    MenuItem {
                        function toTitleCase(str) {
                            return str.replace(
                                        /\w\S*/g,
                                        text => text.charAt(0).toUpperCase() + text.substring(1).toLowerCase()
                                        );
                        }

                        text: "&" + toTitleCase(model.name)
                        checkable: true
                        checked: settings.accent === model.name
                        onCheckedChanged: {
                            if (checked)
                                Constants.setAccent(model.name)
                        }
                    }
                }
            }

            Menu {
                title: qsTr("&Custom Accents")
                MenuItem {
                    text: "&Edit Accents..."
                    onTriggered: screen.editAccents()
                }
                MenuItem {
                    text: "Export Accents..."
                    onTriggered: screen.exportAccentsAction()
                }
                MenuItem {
                    text: "Import Accents..."
                    onTriggered: screen.importAccentsAction()
                }
            }
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
                        onTriggered: tlm.load(modelData)
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
                text: qsTr("Tab &Size")
                onTriggered: screen.tabSize()
                shortcut: "Ctrl+R"
            }

            Action {
                text: qsTr("&Rename Tab")
                onTriggered: screen.renameTab()
                shortcut: "Ctrl+N"
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
    }

    MainScreen {
        id: screen
        anchors.fill: parent
    }
}


