import QtQuick
import QtQuick.Controls

import QFRCDashboard

Rectangle {
    color: Constants.accent
    topLeftRadius: 20
    topRightRadius: 20

    height: 40
    width: parent.width

    Text {
        id: txt

        text: "Configure Widget"
        font.pixelSize: 24
        font.bold: true

        color: "white"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors {
            fill: parent
            margins: 8
        }
    }
}
