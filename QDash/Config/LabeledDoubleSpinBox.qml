// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import Carboxyl.Clover

import Carboxyl.Contour

CarboxylLabeledDoubleSpinBox {
    id: spin

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    property var bindTarget: widget

    font.pixelSize: 18

    from: -1E9
    to: 1E9

    function open() {
        value = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = value
    }
}
