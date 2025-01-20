import QtQuick

import QFRCDashboard

Rectangle {
    id: rem

    signal clicked
    signal activated

    color: mouseArea.containsMouse ? "#82bbff" : (ListView.isCurrentItem ? "#00aaff" : Constants.palette.dialogBg)
    radius: 5
    opacity: 1

    Behavior on color {
        ColorAnimation {
            duration: 250
        }
    }

    border {
        color: Constants.palette.text
        width: 1
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onDoubleClicked: activated()

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
