import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import QDash.Constants
import QDash.Config

ColumnLayout {
    spacing: 15

    function accept() {
        scale.accept()
        resize.accept()
    }

    function open() {
        scale.open()
        resize.open()
    }

    LabeledCheckbox {
        id: resize
        label: "Resize to Driver Station?"

        bindTarget: settings
        bindedProperty: "resizeToDS"
    }
}
