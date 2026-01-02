// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4
import QtQuick.Dialogs

import Carboxyl.Clover
import QDash.Config

ColumnLayout {
    spacing: 15

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
        label: "Load Recent Files?"

        bindTarget: QDashSettings
        bindedProperty: "loadRecent"
    }

    LabeledCheckbox {
        id: disable
        label: "Disable Widgets on Disconnect?"

        bindTarget: QDashSettings
        bindedProperty: "disableWidgets"
    }

    LabeledCheckbox {
        id: resize
        label: "Resize to Driver Station?"

        bindTarget: QDashSettings
        bindedProperty: "resizeToDS"
    }

    LabeledIndexComboBox {
        Layout.fillWidth: true

        implicitHeight: 45
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        id: level
        label: "Log Level"

        model: ["Critical", "Warning", "Info", "Debug"]

        hoverEnabled: true

        ToolTip.visible: hovered
        ToolTip.text: "The log file is located in the local data location."

        bindTarget: QDashSettings
        bindedProperty: "logLevel"
    }
}
