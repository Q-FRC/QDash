// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import QtQuick.Layouts

RowLayout {

    /** the target to bind the property to */
    property var bindTarget: widget

    /** what property to bind to */
    required property string bindedProperty
    required property string label

    function accept() {
        textField.accept()
    }

    function open() {
        textField.open()
    }

    ColorDialog {
        id: colorDialog

        selectedColor: textField.text

        onAccepted: textField.text = colorDialog.selectedColor
    }

    LabeledTextField {
        id: textField

        bindTarget: parent.bindTarget
        bindedProperty: parent.bindedProperty
        label: parent.label
    }

    Button {
        Layout.fillWidth: true
        font.pixelSize: 18
        text: "Pick"

        onClicked: colorDialog.open()
    }
}
