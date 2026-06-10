// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 6.8
import QtQuick.Controls 6.8
import QtQuick.Layouts 6.8

import Carboxyl.Clover
import QDash.Dialogs

import Carboxyl.Contour

Loader {
    id: loader
    active: false
    asynchronous: true
    onLoaded: item.open()

    sourceComponent: active ? src : undefined

    function open() {
        active = true;
    }

    property Component src: CarboxylDialog {
        id: serverDialog

        implicitWidth: 575
        implicitHeight: 475
        title: "Settings"

        popupType: Popup.Window

        onClosed: loader.active = false

        onAccepted: {
            server.accept();
            appearance.accept();
            misc.accept();

            QDashSettings.reconnect();

            if (QDashApplication.shouldReload) {
                QDashApplication.shouldReload = false;

                styleWarnDialog.open();
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Shortcut {
            onActivated: reject()
            sequence: Qt.Key_Escape
        }

        onAboutToShow: {
            server.open();
            misc.open();
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

            contentHeight: 80

            Repeater {
                model: ["Network", "Appearance", "Miscellaneous"]

                CarboxylTabButton {
                    id: btn
                    required property string modelData
                    required property int index

                    // TODO(crueter): Why
                    Component.onCompleted: tabBar.setCurrentIndex(0)

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
}
