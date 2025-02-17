import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import QFRCDashboard

Rectangle {
    signal minimize
    signal maximize
    signal close
    signal beginDrag

    color: Constants.accent
    height: 30

    MouseArea {
        anchors.fill: parent
        onPressed: beginDrag()
    }

    RowLayout {
        anchors.fill: parent
        anchors.topMargin: -10
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        spacing: 10

        Image {
            height: 30
            source: "qrc:/QFRCDashboard"
            sourceSize.height: 24
            sourceSize.width: 24

            fillMode: Image.PreserveAspectFit
        }

        MenuBarItem {
            text: qsTr("&File")
            onClicked: menu.open()

            menu: Menu {
                // contentWidth: 175
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
                    text: qsTr("Remote &Layouts...")
                    onTriggered: remoteLayouts.openDialog()
                    shortcut: "Ctrl+L"
                }
            }
        }

        MenuBarItem {
            text: qsTr("&Edit")
            onClicked: menu.open()

            menu: Menu {
                title: parent.text

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
            }
        }

        MenuBarItem {
            text: qsTr("&Tab")
            onClicked: menu.open()

            menu: Menu {
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
        }

        MenuBarItem {
            text: qsTr("&About")
            onTriggered: about.open()
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            height: 30
            background: Item {}

            icon.source: "qrc:/Minimize"
            icon.width: 24
            icon.height: 24
            icon.color: Constants.palette.text

            onClicked: minimize()
        }

        Button {
            height: 30
            background: Item {}

            property bool maximized: false

            icon.source: maximized ? "qrc:/Unmaximize" : "qrc:/Window"
            icon.width: 24
            icon.height: 24
            icon.color: Constants.palette.text

            onClicked: {
                maximized = !maximized

                maximize()
            }
        }

        Button {
            height: 30
            background: Item {}

            icon.source: "qrc:/Close"
            icon.width: 24
            icon.height: 24
            icon.color: Constants.palette.text

            onClicked: close()
        }
    }

    Text {
        height: 30
        width: parent.width
        color: Constants.palette.text
        text: "QFRCDashboard"
        font.pixelSize: 18

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
