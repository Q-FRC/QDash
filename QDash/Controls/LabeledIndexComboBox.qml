// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick.Layouts
import Carboxyl.Contour

CarboxylLabeledComboBox {
    id: combo
    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    property var bindTarget: widget
    font.pixelSize: 18

    implicitHeight: 38

    Layout.fillWidth: true

    function open() {
        currentIndex = bindTarget[bindedProperty];
    }

    function accept() {
        bindTarget[bindedProperty] = currentIndex;
    }
}
