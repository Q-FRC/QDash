import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes 2.15

import QFRCDashboard

ComboBox {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    /** choices for the combobox */
    required property var choices

    id: combo
    model: choices
    font.pixelSize: 15

    height: 50

    function open() {
        currentIndex = indexOfValue(bindTarget[bindedProperty])
    }

    function accept() {
        bindTarget[bindedProperty] = currentText
    }

    delegate: ItemDelegate {
        id: delegate

        width: combo.width
        contentItem: Text {
            text: modelData
            color: Constants.palette.text
            font.pixelSize: 15
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: combo.highlightedIndex === index
    }

    Text {
        id: floatingLabel
        text: label
        color: Constants.palette.text

        font.pixelSize: 15

        anchors {
            left: parent.left
            bottom: parent.top

            bottomMargin: -2
            leftMargin: 10
        }
    }
}
