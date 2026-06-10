// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import Carboxyl.Contour
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

CarboxylLabeledSpinBox {
    id: spin

    /** the target to bind the property to */
    property var bindTarget: widget

    /** what property to bind to */
    required property string bindedProperty

    function accept() {
        bindTarget[bindedProperty] = value
    }

    function open() {
        value = bindTarget[bindedProperty]
    }

    Layout.fillWidth: true
    font.pixelSize: 18
    from: 0
    to: 1E9
}
