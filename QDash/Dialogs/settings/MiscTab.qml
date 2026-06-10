// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QDash.Controls
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

ColumnLayout {
    readonly property bool centered: controlWidth < width
    readonly property int controlWidth: Math.min(width, 350)

    // anchors
    function accept() {
        load.accept()
        level.accept()
        disable.accept()
        resize.accept()
    }

    function open() {
        load.open()
        level.open()
        disable.open()
        resize.open()
    }

    spacing: 15

    LabeledCheckbox {
        id: load

        Layout.alignment: centered ? Qt.AlignCenter : Qt.AlignLeft
        bindTarget: QDashSettings
        bindedProperty: "loadRecent"
        implicitHeight: 45
        implicitWidth: controlWidth
        label: qsTr("Load Recent Files?")
    }

    LabeledCheckbox {
        id: disable

        Layout.alignment: centered ? Qt.AlignCenter : Qt.AlignLeft
        bindTarget: QDashSettings
        bindedProperty: "disableWidgets"
        implicitHeight: 45
        implicitWidth: controlWidth
        label: qsTr("Disable Widgets on Disconnect?")
    }

    LabeledCheckbox {
        id: resize

        Layout.alignment: centered ? Qt.AlignCenter : Qt.AlignLeft
        bindTarget: QDashSettings
        bindedProperty: "resizeToDS"
        implicitHeight: 45
        implicitWidth: controlWidth
        label: qsTr("Resize to Driver Station?")
    }

    RowLayout {
        Layout.alignment: Qt.AlignCenter
        implicitHeight: 45
        implicitWidth: Math.min(450, parent.width)

        LabeledIndexComboBox {
            id: level

            ToolTip.text: "The log file is located in the local data location."
            ToolTip.visible: hovered
            bindTarget: QDashSettings
            bindedProperty: "logLevel"
            font.pixelSize: 20
            hoverEnabled: true
            implicitHeight: 45
            label: "Log Level"
            model: [qsTr("Critical"), qsTr("Warning"), qsTr("Info"), qsTr("Debug")]
        }

        Button {
            Layout.topMargin: 2
            font.pixelSize: 20
            implicitHeight: 40
            text: qsTr("Open Log Location")

            onClicked: logs.openLogLocation()
        }
    }
}
