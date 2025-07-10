import QtQuick
import QtQuick.Controls.Material 2.15

import QDash

MenuBar {
    background: Rectangle {
        implicitHeight: 30
        color: Constants.palette.button
    }

    function fixAmpersands(originalText) {
        var regex = /&(\w)/g
        return originalText.replace(regex, "<u>$1</u>")
    }

    delegate: MenuBarItem {
        id: control

        font.pixelSize: 16

        background: Rectangle {
            color: control.down || control.hovered
                   || control.highlighted ? Constants.buttonHighlighted : Constants.button
        }

        contentItem: Text {
            text: fixAmpersands(control.text)
            color: Constants.palette.text
            font: control.font
        }
    }
}
