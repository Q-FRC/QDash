import QtQuick
import QtQuick.Controls

import QFRCDashboard

AnimatedDialog {
    title: "About QFRCDashboard"

    width: 325 * Constants.scalar
    height: 375 * Constants.scalar

    Text {
        color: Constants.palette.text
        font.pixelSize: (14 * Constants.scalar)

        text: "<p>QFRCDashboard is a reliable, high-performance FRC dashboard with " +
              "a low resource cost, suited for low-end computers and for maximizing " +
              "Driver Station resources.</p>" +
              buildConfig.buildInfo() +
              "Copyleft 2023-2024 Q-FRC"

        wrapMode: Text.WordWrap
        textFormat: Text.RichText

        anchors.fill: parent
    }

    standardButtons: "Close"
}
