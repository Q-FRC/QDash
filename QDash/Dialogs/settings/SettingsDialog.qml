// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 6.4
import QtQuick.Controls 6.4
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import QDash.Dialogs

import Carboxyl.Contour

NativeDialog {
    id: serverDialog

    width: 575
    height: 475
    title: "Settings"

    onAccepted: {
        server.accept()
        appearance.accept()
        misc.accept()

        QDashSettings.reconnect()

        if (QDashApplication.shouldReload) {
            QDashApplication.shouldReload = false

            styleWarnDialog.open()
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    Shortcut {
        onActivated: reject()
        sequence: Qt.Key_Escape
    }

    function open() {
        show()

        server.open()
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

            // topMargin: 10
            margins: 15
        }

        ServerTab {
            id: server
            clip: true
        }

        AppearanceTab {
            id: appearance
            clip: true
        }

        MiscTab {
            id: misc
            clip: true
        }
    }

    CarboxylTabBar {
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
        }

        Component.onCompleted: currentIndex = 0

        contentHeight: 80

        Repeater {
            model: ["Network", "Appearance", "Miscellaneous"]

            CarboxylTabButton {
                required property string modelData
                required property int index

                id: btn

                text: modelData

                icon.height: 40
                icon.width: 40
                icon.source: "qrc:/" + modelData

                coloredIcon: true
                inlineIcon: false
            }
        }
    }
}
