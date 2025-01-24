import QtQuick
import QtQuick.Controls
import QFRCDashboard

TextField {
    property bool valid: true

    color: Constants.palette.text

    background: Rectangle {
        color: Constants.palette.bg
        width: parent.width
        border.color: valid ? Constants.palette.text : "red"
        border.width: 1
        radius: 5

        Behavior on border.color {
            ColorAnimation {
                duration: 250
            }
        }
    }
}
