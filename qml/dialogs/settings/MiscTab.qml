import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

ColumnLayout {
    spacing: 15 * Constants.scalar

    function accept() {
        load.accept()
        scale.accept()
    }

    function open() {
        load.open()
        scale.open()
    }

    LabeledCheckbox {
        id: load
        label: "Load Recent Files?"

        bindTarget: settings
        bindedProperty: "loadRecent"
    }

    LabeledDoubleSpinBox {
        Layout.fillWidth: true

        hoverEnabled: true
        id: scale
        label: "Custom Scaling Factor?"

        ToolTip.visible: hovered
        ToolTip.text: "Useful for high-DPI screens, this scalar is applied on top of the existing window-size scalar."

        stepSize: 0.1
        from: 0.1
        to: 10.0

        bindTarget: settings
        bindedProperty: "scale"
    }
}
