import QtQuick

import QFRCDashboard

Rectangle {
    id: rem

    signal clicked
    signal activated

    color: Constants.palette.dialogBg
    radius: 5

    border {
        color: Constants.palette.text
        width: 2
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onDoubleClicked: activated()
        onContainsMouseChanged: {
                if (containsMouse) {
                    parent.color = "#2abcff"
                } else {
                    parent.color = Constants.palette.dialogBg
                }
        }

        onClicked: rem.clicked()
    }

    Text {
        anchors.fill: parent
        anchors.leftMargin: 20 * Constants.scalar

        color: Constants.palette.text
        text: model.name
        font.pixelSize: Math.round(18 * Constants.scalar)

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
