// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

CheckBox {
    id: textField

    /** the target to bind the property to */
    property var bindTarget: widget

    /** what property to bind to */
    required property string bindedProperty
    required property string label

    function accept() {
        bindTarget[bindedProperty] = checked
    }

    function open() {
        checked = bindTarget[bindedProperty]
    }

    Layout.fillWidth: true
    font.pixelSize: 18
    indicator.implicitHeight: 28
    indicator.implicitWidth: 28
    text: label
}
