import QtQuick
import QtQuick.Controls.Material

import QDash

AnimatedDialog {
    title: "About QDash"

    width: 325 * Constants.scalar
    height: 375 * Constants.scalar

    Text {
        color: Constants.palette.text
        font.pixelSize: (14 * Constants.scalar)

        text: "<p>QDash is a reliable, high-performance FRC dashboard with "
              + "a low resource cost, suited for low-end computers and for maximizing "
              + "Driver Station resources.</p>" + buildConfig.buildInfo(
                  ) + "Copyleft 2023-2025 Q-FRC and crueter"

        wrapMode: Text.WordWrap
        textFormat: Text.RichText

        anchors.fill: parent

        onLinkActivated: link => Qt.openUrlExternally(link)
    }

    standardButtons: "Close"
}
