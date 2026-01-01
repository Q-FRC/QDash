import QtQuick

import QDash.Constants

Text {
    required property string label
    font.pixelSize: 18
    font.weight: 700
    color: Constants.palette.text
    text: label

    width: parent.width
    horizontalAlignment: Text.AlignLeft
}
