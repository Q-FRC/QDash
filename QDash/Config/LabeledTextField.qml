import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 2.15

import QDash.Constants
import QDash.Fields

BetterTextField {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    id: textField
    font.pixelSize: 18

    function open() {
        text = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = text
    }

    placeholderText: label
}
