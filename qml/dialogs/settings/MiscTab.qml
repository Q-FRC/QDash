import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

ColumnLayout {
    spacing: 15 * Constants.scalar

    function accept() {
        load.accept()
        level.accept()
    }

    function open() {
        load.open()
        level.open()
    }

    LabeledCheckbox {
        id: load
        label: "Load Recent Files?"

        bindTarget: settings
        bindedProperty: "loadRecent"
    }

    LabeledIndexComboBox {
        Layout.fillWidth: true

        id: level
        label: "Log Level"

        choices: ["Critical", "Warning", "Info", "Debug"]

        hoverEnabled: true

        ToolTip.visible: hovered
        ToolTip.text: "The log file is located in the local data location."

        bindTarget: settings
        bindedProperty: "logLevel"
    }
}
