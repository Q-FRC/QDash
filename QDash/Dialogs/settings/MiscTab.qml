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
    readonly property bool centered: controlWidth < width

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

    LabeledCheckbox {
        id: load
        label: qsTr("Load Recent Files?")

        bindTarget: QDashSettings
        bindedProperty: "loadRecent"

        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: centered ? Qt.AlignCenter : Qt.AlignLeft
    }

    LabeledCheckbox {
        id: disable
        label: qsTr("Disable Widgets on Disconnect?")

        bindTarget: QDashSettings
        bindedProperty: "disableWidgets"

        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: centered ? Qt.AlignCenter : Qt.AlignLeft
    }

    LabeledCheckbox {
        id: resize
        label: qsTr("Resize to Driver Station?")

        bindTarget: QDashSettings
        bindedProperty: "resizeToDS"

        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: centered ? Qt.AlignCenter : Qt.AlignLeft
    }

    RowLayout {
        Layout.alignment: Qt.AlignCenter
        implicitHeight: 45
        implicitWidth: Math.min(450, parent.width)

        LabeledIndexComboBox {
            font.pixelSize: 20
            implicitHeight: 45

            id: level
            label: "Log Level"

            model: [qsTr("Critical"), qsTr("Warning"), qsTr(
                    "Info"), qsTr("Debug")]

            hoverEnabled: true

            ToolTip.visible: hovered
            ToolTip.text: "The log file is located in the local data location."

            bindTarget: QDashSettings
            bindedProperty: "logLevel"
        }

        Button {
            font.pixelSize: 20
            implicitHeight: 40

            Layout.topMargin: 2

            text: qsTr("Open Log Location")
            onClicked: logs.openLogLocation()
        }
    }
}
