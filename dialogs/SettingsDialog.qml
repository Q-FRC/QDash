import QtQuick 6.7
import QtQuick.Controls 6.6
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

Dialog {
    id: serverDialog

    height: 350
    width: 475

    background: Rectangle {
        color: Constants.palette.dialogBg

        radius: 12
    }

    anchors.centerIn: parent

    onAccepted: {
        server.accept()
        appearance.accept()
        misc.accept()
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    Shortcut {
        onActivated: reject()
        sequence: Qt.Key_Escape
    }

    function openDialog() {
        open()
        server.open()
        appearance.open()
        misc.open()
    }

    SwipeView {
        id: swipe
        currentIndex: tabBar.currentIndex
        clip: true

        anchors {
            top: tabBar.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        ServerTab {
            anchors.leftMargin: 5

            id: server
            clip: true
        }

        AppearanceTab {
            anchors.leftMargin: 5

            id: appearance
            clip: true
        }

        MiscTab {
            anchors.leftMargin: 5

            id: misc
            clip: true
        }
    }

    TabBar {
        id: tabBar
        currentIndex: swipe.currentIndex
        position: TabBar.Header

        background: Rectangle {
            color: "transparent"
        }

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right

            leftMargin: -24
            rightMargin: -24
            topMargin: -23
        }

        Repeater {
            model: ["Network", "Appearance", "Miscellaneous"]

            SettingsTabButton {
                required property string modelData
                required property int index

                label: modelData
            }
        }
    }
}
