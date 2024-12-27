import QtQuick
import QtQuick.Controls

import QFRCDashboard

AnimatedDialog {
    title: "Close Tab?"

    width: 200
    height: 170

    anchors.centerIn: Overlay.overlay

    Text {
        font.pixelSize: 15
        color: Constants.palette.text
        text: "Are you sure you want to close this tab?"

        anchors.fill: parent
    }

    standardButtons: Dialog.Yes | Dialog.No
}
