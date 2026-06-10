// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Controls
import QtQuick
import QtQuick.Controls

Loader {
    id: loader

    property Component src: CarboxylDialog {
        id: tabNameDialog

        implicitWidth: 300
        popupType: Popup.Window
        standardButtons: Dialog.Ok | Dialog.Cancel
        title: "New Tab"

        onAboutToShow: {
            tabName.focus = true
            tabName.text = ""
        }
        onAccepted: {
            loader.text = tabName.text
            loader.accepted()
        }
        onClosed: loader.active = false

        Shortcut {
            sequence: Qt.Key_Escape

            onActivated: reject()
        }

        Shortcut {
            sequence: Qt.Key_Return

            onActivated: accept()
        }

        LabeledTextField {
            id: tabName

            bindTarget: parent
            bindedProperty: "text"
            font.pixelSize: 24
            implicitHeight: 48
            label: "Tab Name"

            onAccepted: tabNameDialog.accept()

            anchors {
                left: parent.left
                margins: 10
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }
    }
    property string text

    signal accepted

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: item.open()
}
