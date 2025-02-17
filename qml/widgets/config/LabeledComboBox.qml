import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Shapes 2.15

import QFRCDashboard

BetterComboBox {
    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    /** choices for the combobox */
    required property var choices

    id: combo
    model: choices
    font.pixelSize: 18 * Constants.scalar

    implicitHeight: 40 * Constants.scalar

    function open() {
        currentIndex = indexOfValue(bindTarget[bindedProperty])
    }

    function accept() {
        bindTarget[bindedProperty] = currentText
    }
}
