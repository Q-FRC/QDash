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

    height: tabName.implicitHeight + footer.height + 10
    width: 200

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
        anchors.fill: parent
        anchors.topMargin: 10

        id: tabName
        font.pixelSize: Math.round(20)

        onAccepted: tabNameDialog.accept()

        label: "Tab Name"

        bindTarget: parent
        bindedProperty: "text"
    }
}
