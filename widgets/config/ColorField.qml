import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Shapes 2.15
import QtQuick.Dialogs

import QFRCDashboard

RowLayout {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    function open() {
        textField.open()
    }

    function accept() {
        textField.accept()
    }

    ColorDialog {
        id: colorDialog

        selectedColor: textField.text
        onAccepted: textField.text = colorDialog.selectedColor
    }

    LabeledTextField {
        id: textField
        Layout.fillWidth: true

        label: parent.label
        bindedProperty: parent.bindedProperty
        bindTarget: parent.bindTarget
    }

    Button {
        Layout.fillWidth: true
        text: "Pick"
        font.pixelSize: 18 * Constants.scalar

        onClicked: {
            colorDialog.open()
        }
    }
}
