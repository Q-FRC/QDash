import QtQuick
import QtQuick.Controls.Material

import QFRCDashboard

Rectangle {
    color: Constants.accent
    topLeftRadius: 20 * Constants.scalar
    topRightRadius: 20 * Constants.scalar

    height: 40 * Constants.scalar
    width: parent.width

    Text {
        id: txt

        text: "Configure Widget"
        font.pixelSize: 24 * Constants.scalar
        font.bold: true

        color: "white"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors {
            fill: parent
            margins: 8 * Constants.scalar
        }
    }
}
