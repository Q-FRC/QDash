// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import Carboxyl.Contour
import QDash.Dialogs
import QtQuick 6.8
import QtQuick.Controls 6.8
import QtQuick.Layouts 6.8

Loader {
    id: loader

    property Component src: CarboxylDialog {
        id: serverDialog

        implicitHeight: 475
        implicitWidth: 575
        popupType: Popup.Window
        standardButtons: Dialog.Ok | Dialog.Cancel
        title: "Settings"

        onAboutToShow: {
            server.open()
            misc.open()
        }
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
        onClosed: loader.active = false

        Shortcut {
            sequence: Qt.Key_Escape

            onActivated: reject()
        }

        SwipeView {
            id: swipe

            clip: true
            currentIndex: tabBar.currentIndex

            anchors {
                bottom: parent.bottom
                left: parent.left
                margins: 15
                right: parent.right
                top: tabBar.bottom
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

            contentHeight: 80
            currentIndex: swipe.currentIndex
            position: TabBar.Header

            background: Rectangle {
                color: "transparent"
            }

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            Repeater {
                model: ["Network", "Appearance", "Miscellaneous"]

                CarboxylTabButton {
                    id: btn

                    required property int index
                    required property string modelData

                    coloredIcon: true
                    icon.height: 40
                    icon.source: "qrc:/" + modelData
                    icon.width: 40
                    inlineIcon: false
                    text: modelData

                    // TODO(crueter): Why
                    Component.onCompleted: tabBar.setCurrentIndex(0)
                }
            }
        }
    }

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: item.open()
}
