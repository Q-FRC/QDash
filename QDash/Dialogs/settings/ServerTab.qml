// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import QDash.Config

ColumnLayout {
    spacing: 15

    readonly property int controlWidth: Math.min(width, 350)

    function accept() {
        team.accept()
        ip.accept()
        mode.accept()
    }

    function open() {
        team.open()
        ip.open()
        mode.open()
    }

    LabeledSpinBox {
        id: team
        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        from: 0
        to: 99999

        label: qsTr("Team Number")

        bindedProperty: "teamNumber"
        bindTarget: QDashSettings
    }

    LabeledTextField {
        id: ip
        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        label: qsTr("IP Address")

        horizontalAlignment: "AlignHCenter"

        bindedProperty: "ip"
        bindTarget: QDashSettings

        validator: RegularExpressionValidator {
            regularExpression: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
        }
    }

    LabeledIndexComboBox {
        id: mode
        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        label: qsTr("Connection Mode")

        bindedProperty: "connMode"
        bindTarget: QDashSettings

        model: ["IP Address", "Team Number", "Driver Station"]
    }
}
