import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import QFRCDashboard

ColumnLayout {
    spacing: 15 * Constants.scalar

    function accept() {
        scale.accept()
        resize.accept()
    }

    function open() {
        scale.open()
        resize.open()
    }

    LabeledDoubleSpinBox {
        Layout.fillWidth: true

        hoverEnabled: true
        id: scale
        label: "Custom Scaling Factor"

        ToolTip.visible: hovered
        ToolTip.text: "Useful for high-DPI screens, this scalar is applied on top of the existing window-size scalar."

        stepSize: 0.1
        from: 0.1
        to: 10.0

        bindTarget: settings
        bindedProperty: "scale"
    }

    LabeledCheckbox {
        id: resize
        label: "Resize to Driver Station?"

        bindTarget: settings
        bindedProperty: "resizeToDS"
    }
}
