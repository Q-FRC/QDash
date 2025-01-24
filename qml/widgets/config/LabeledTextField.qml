import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QtMultimedia

import QFRCDashboard

BetterTextField {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    id: textField
    font.pixelSize: 18 * Constants.scalar

    function open() {
        text = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = text
    }

    Text {
        id: floatingLabel
        text: label
        color: Constants.palette.text

        font.pixelSize: 15 * Constants.scalar

        anchors {
            left: textField.left
            bottom: textField.top

            bottomMargin: -2
            leftMargin: 10 * Constants.scalar
        }
    }
}
