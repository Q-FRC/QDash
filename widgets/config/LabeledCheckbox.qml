import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QtMultimedia

import QFRCDashboard

CheckBox {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    id: textField
    font.pixelSize: 18

    function open() {
        checked = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = checked
    }

    text: label
}
