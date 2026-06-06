// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Controls

Loader {
    id: loader
    active: false
    asynchronous: true
    onLoaded: item.open()

    sourceComponent: active ? src : undefined

    property string text

    signal accepted

    function open() {
        active = true
    }

    property Component src: CarboxylDialog {
        id: tabNameDialog

        title: "New Tab"

        implicitWidth: 300

        popupType: Popup.Window

        onAboutToShow: {
            tabName.focus = true
            tabName.text = ""
        }

        onAccepted: {
            loader.text = tabName.text
            loader.accepted()
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Shortcut {
            onActivated: reject()
            sequence: Qt.Key_Escape
        }

        Shortcut {
            onActivated: accept()
            sequence: Qt.Key_Return
        }

        LabeledTextField {
            id: tabName
            font.pixelSize: 24
            implicitHeight: 48

            anchors {
                left: parent.left
                right: parent.right

                verticalCenter: parent.verticalCenter
                margins: 10
            }

            onAccepted: tabNameDialog.accept()

            label: "Tab Name"

            bindTarget: parent
            bindedProperty: "text"
        }

        onClosed: loader.active = false
    }
}
