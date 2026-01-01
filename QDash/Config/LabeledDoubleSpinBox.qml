// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QDash.Constants
import QDash.Fields

BetterDoubleSpinBox {
    id: spin
    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    from: -1E9
    to: 1E9

    height: 50

    font.pixelSize: 18

    function open() {
        value = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = value
    }
}
