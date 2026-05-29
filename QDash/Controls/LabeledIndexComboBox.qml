// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import Carboxyl.Contour

CarboxylLabeledComboBox {
    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    property var bindTarget: widget

    id: combo
    font.pixelSize: 18

    implicitHeight: 38

    function open() {
        currentIndex = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = currentIndex
    }
}
