import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QtMultimedia

import QFRCDashboard

BetterSpinBox {
    id: spin
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    font.pixelSize: 18 * Constants.scalar

    from: 0
    to: 1E9

    function open() {
        value = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = value
    }

    Text {
        id: floatingLabel
        text: label
        color: Constants.palette.text

        font.pixelSize: 15 * Constants.scalar

        anchors {
            left: spin.left
            bottom: spin.top

            bottomMargin: -2
            leftMargin: 10 * Constants.scalar
        }
    }
}
