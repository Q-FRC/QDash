// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QDash.Constants
import QDash.Config

ColumnLayout {
    spacing: 15

    function accept() {
        resize.accept()
    }

    function open() {
        resize.open()
    }

    LabeledCheckbox {
        id: resize
        label: "Resize to Driver Station?"

        bindTarget: settings
        bindedProperty: "resizeToDS"
    }
}
