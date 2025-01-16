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

    required property var choices

    id: combo
    model: choices
    font.pixelSize: Math.round(15 * Constants.scalar)

    function toTitleCase(str) {
        return str.replace(
                    /\w\S*/g,
                    text => text.charAt(0).toUpperCase() + text.substring(1).toLowerCase()
                    );
    }

    function open() {
        currentIndex = indexOfValue(toTitleCase(bindTarget[bindedProperty]))
    }

    function accept() {
        bindTarget[bindedProperty] = currentText.toLowerCase()
    }

    delegate: ItemDelegate {
        id: delegate

        width: combo.width
        contentItem: Text {
            text: modelData
            color: Constants.palette.text
            font.pixelSize: Math.round(15 * Constants.scalar)
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: combo.highlightedIndex === index
    }

    Text {
        id: floatingLabel
        text: label
        color: Constants.palette.text

        font.pixelSize: Math.round(15 * Constants.scalar)

        anchors {
            left: parent.left
            bottom: parent.top

            bottomMargin: -2
            leftMargin: 10
        }
    }
}
