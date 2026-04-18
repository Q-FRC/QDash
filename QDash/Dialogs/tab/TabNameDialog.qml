// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Config

NativeDialog {
    id: tabNameDialog

    title: "New Tab"

    height: tabName.implicitHeight + footer.height + 40
    width: 300

    property string text

    function open() {
        show()
        tabName.focus = true
        tabName.text = ""
    }

    onAccepted: text = tabName.text

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
}
