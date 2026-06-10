// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import Carboxyl.Contour
import QtQuick.Layouts

CarboxylLabeledComboBox {
    id: combo

    /** the target to bind the property to */
    property var bindTarget: widget

    /** what property to bind to */
    required property string bindedProperty

    function accept() {
        bindTarget[bindedProperty] = currentText
    }

    function open() {
        currentIndex = indexOfValue(bindTarget[bindedProperty])
    }

    Layout.fillWidth: true
    font.pixelSize: 18
    implicitHeight: 38
}
