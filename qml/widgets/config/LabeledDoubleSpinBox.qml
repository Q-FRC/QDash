import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QtMultimedia

import QFRCDashboard

BetterDoubleSpinBox {
    id: spin
    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    from: -1E9
    to: 1E9

    height: 50 * Constants.scalar

    font.pixelSize: 18 * Constants.scalar

    function open() {
        value = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = value
    }
}
