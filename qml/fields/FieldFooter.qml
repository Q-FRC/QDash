import QtQuick
import QtQuick.Controls.Material
import QFRCDashboard

Rectangle {
    property bool valid

    height: 2 * Constants.scalar
    color: valid ? Constants.palette.text : "red"
    width: parent.width

    anchors {
        bottom: parent.bottom
        left: parent.left
    }

    Behavior on color {
        ColorAnimation {
            duration: 250
        }
    }
}
