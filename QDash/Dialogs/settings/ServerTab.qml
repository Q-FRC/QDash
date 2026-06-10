// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QDash.Controls
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

ColumnLayout {
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

    spacing: 15

    LabeledSpinBox {
        id: team

        Layout.alignment: Qt.AlignCenter
        bindTarget: QDashSettings
        bindedProperty: "teamNumber"
        font.pixelSize: 20
        from: 0
        implicitHeight: 45
        implicitWidth: controlWidth
        label: qsTr("Team Number")
        to: 99999
    }

    LabeledTextField {
        id: ip

        Layout.alignment: Qt.AlignCenter
        bindTarget: QDashSettings
        bindedProperty: "ip"
        font.pixelSize: 20
        horizontalAlignment: "AlignHCenter"
        implicitHeight: 45
        implicitWidth: controlWidth
        label: qsTr("IP Address")

        validator: RegularExpressionValidator {
            regularExpression: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
        }
    }

    LabeledIndexComboBox {
        id: mode

        Layout.alignment: Qt.AlignCenter
        bindTarget: QDashSettings
        bindedProperty: "connMode"
        font.pixelSize: 20
        implicitHeight: 45
        implicitWidth: controlWidth
        label: qsTr("Connection Mode")
        model: ["IP Address", "Team Number", "Driver Station"]
    }
}
