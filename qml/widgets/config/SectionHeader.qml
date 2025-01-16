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
    }

    Rectangle {
        height: 2
        width: 350 * Constants.scalar
        color: Constants.palette.text
    }
}
