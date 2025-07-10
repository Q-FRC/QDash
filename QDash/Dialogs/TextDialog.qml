import QtQuick

import QDash.Constants

AnimatedDialog {
    id: dialog
    required property string text
    Text {
        color: Constants.palette.text
        font.pixelSize: (14)

        text: dialog.text

        wrapMode: Text.WordWrap
        textFormat: Text.RichText

        anchors.fill: parent

        onLinkActivated: link => Qt.openUrlExternally(link)
    }
}
