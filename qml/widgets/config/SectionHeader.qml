import QtQuick

import QFRCDashboard

Column {
    width: parent.width

    /** What text to display */
    required property string label

    Text {
        font.pixelSize: 18 * Constants.scalar
        font.weight: 700
        color: Constants.palette.text
        text: label

        width: parent.width
        horizontalAlignment: Text.AlignLeft
    }
}
