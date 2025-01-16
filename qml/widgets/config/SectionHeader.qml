import QtQuick

import QFRCDashboard

Column {
    width: parent.width

    /** What text to display */
    required property string label

    Text {
        font.pixelSize: 18 * Constants.scalar
        font.bold: true
        color: Constants.palette.text
        text: label

        width: parent.width
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        height: 2
        width: parent.width
        color: Constants.palette.text
    }
}
