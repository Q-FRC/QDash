import QtQuick
import QtQuick.Controls

import QFRCDashboard

AnimatedDialog {
    title: "Close Tab?"

    width: 250 * Constants.scalar
    height: 185 * Constants.scalar

    anchors.centerIn: Overlay.overlay

    Text {
        font.pixelSize: Math.round(15 * Constants.scalar)
        color: Constants.palette.text
        text: "Are you sure you want to close this tab?"

        wrapMode: Text.WordWrap
        anchors.fill: parent
    }

    standardButtons: Dialog.Yes | Dialog.No
}
