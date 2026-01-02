// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import Carboxyl.Clover

import Carboxyl.Contour

CarboxylLabeledTextField {
    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    property var bindTarget: widget

    id: textField
    font.pixelSize: 18

    function open() {
        text = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = text
    }
}
