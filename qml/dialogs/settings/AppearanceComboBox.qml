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

    required property var choices

    id: combo
    model: choices
    font.pixelSize: Math.round(18 * Constants.scalar)

    function toTitleCase(str) {
        return str.replace(/\w\S*/g, text => text.charAt(0).toUpperCase(
                               ) + text.substring(1).toLowerCase())
    }

    function open() {
        currentIndex = indexOfValue(toTitleCase(bindTarget[bindedProperty]))
    }

    function accept() {
        bindTarget[bindedProperty] = currentText.toLowerCase()
    }
}
