import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 2.15

import QDash.Constants

CheckBox {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    id: textField
    font.pixelSize: 18

    indicator.implicitWidth: 28
    indicator.implicitHeight: 28

    function open() {
        checked = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = checked
    }

    text: label
}
