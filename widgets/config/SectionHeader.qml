import QtQuick

import QFRCDashboard

Text {
    width: parent.width

    /** What text to display */
    required property string label

    font.pixelSize: 18
    font.bold: true
    color: Constants.palette.text
    text: label

    Rectangle {
        height: 2
        width: 350
        color: Constants.palette.text

        anchors {
            top: parent.bottom
            left: parent.left

            margins: 2
        }
    }
}
