import QtQuick
import QtQuick.Controls.Material 2.15

import QDash.Constants

Menu {
    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: Constants.palette.button

        radius: 10
    }

    function fixAmpersands(originalText) {
        var regex = /&(\w)/g
        return originalText.replace(regex, "<u>$1</u>")
    }

    delegate: MenuItem {
        id: control

        font.pixelSize: 14

        background: Rectangle {
            color: control.down || control.hovered
                   || control.highlighted ? Constants.palette.buttonHighlighted : Constants.palette.button
        }

        contentItem: Item {
            Text {
                anchors {
                    left: parent.left
                    leftMargin: 5 + (control.checkable ? control.indicator.width : 0)
                    verticalCenter: parent.verticalCenter
                }

                text: fixAmpersands(control.text)
                color: control.enabled ? Constants.palette.text : Constants.palette.disabledText
                font: control.font
            }

            Text {
                anchors {
                    right: parent.right
                    rightMargin: 5
                    verticalCenter: parent.verticalCenter
                }

                Component.onCompleted: if (control.action != null
                                               && typeof control.action.shortcut !== 'undefined')
                                           text = control.action.shortcut

                color: control.enabled ? Constants.palette.text : Constants.palette.disabledText
                font: control.font
            }
        }
    }
}
