// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import QDash.Config

ColumnLayout {
    spacing: 5

    function accept() {
        team.accept()
        ip.accept()
        QDashSettings.mode = mode.currentIndex
    }

    function open() {
        team.open()
        ip.open()
        mode.currentIndex = QDashSettings.connMode
    }

    LabeledSpinBox {
        id: team
        implicitHeight: 45
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        from: 0
        to: 99999

        label: "Team Number"

        bindedProperty: "teamNumber"
        bindTarget: QDashSettings
    }

    LabeledTextField {
        id: ip
        implicitHeight: 45
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        label: "IP Address"

        horizontalAlignment: "AlignHCenter"

        bindedProperty: "ip"
        bindTarget: QDashSettings

        validator: RegularExpressionValidator {
            regularExpression: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
        }
    }

    LabeledComboBox {
        id: mode
        implicitHeight: 45
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        label: "Connection Mode"

        bindedProperty: "mode"
        bindTarget: QDashSettings

        model: ["IP Address", "Team Number", "Driver Station"]
    }
}
