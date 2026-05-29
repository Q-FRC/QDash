// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

CheckBox {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    property var bindTarget: widget

    id: textField
    font.pixelSize: 18

    indicator.implicitWidth: 28
    indicator.implicitHeight: 28

    function open() {
        checked = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = checked
    }

    text: label
}
