import QtQuick 6.7
import QtQuick.Controls 6.6
import QtQuick.Layouts 6.6

import QFRCDashboard

AnimatedDialog {
    id: serverDialog

    height: 475 * Constants.scalar
    width: 550 * Constants.scalar

    title: "Settings"

    onAccepted: {
        server.accept()
        appearance.accept()
        misc.accept()
        windowTab.accept()
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
        windowTab.open()
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

            topMargin: 10
        }

        ServerTab {
            id: server
            clip: true
        }

        AppearanceTab {
            id: appearance
            clip: true
        }

        WindowTab {
            id: windowTab
            clip: true
        }

        MiscTab {
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
            model: ["Network", "Appearance", "Window", "Miscellaneous"]

            SettingsTabButton {
                required property string modelData
                required property int index

                label: modelData

                onClicked: swipe.currentIndex = index
            }
        }
    }
}
