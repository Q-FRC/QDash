// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Items

DoubleSpinBox {
    id: dsb

    property bool connected: true
    property bool valid: true
    property string label: ""
    enabled: connected

    contentItem: BetterSpinBox {
        valid: parent.valid
        label: parent.label
        connected: parent.connected
    }
}
