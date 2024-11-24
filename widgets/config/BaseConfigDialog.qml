import QtQuick
import QtQuick.Controls

import QFRCDashboard

Dialog {
    id: config

    property int headerHeight: 40

    width: 375
    spacing: 10

    standardButtons: Dialog.Ok | Dialog.Cancel

    TitleComponent {}

    background: Rectangle {
        color: "transparent"
    }

    contentItem: Rectangle {
        color: Constants.palette.dialogBg
        radius: 20
        anchors.fill: parent
    }

    parent: Overlay.overlay
    anchors.centerIn: parent
}
