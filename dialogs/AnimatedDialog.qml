import QtQuick
import QtQuick.Controls

import QFRCDashboard

Dialog {
    id: dia

    property int radius: 12
    property bool colorful: true

    anchors.centerIn: Overlay.overlay

    enter: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 200

            from: 0.0
            to: 1.0
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 200

            from: 1.0
            to: 0.0
        }
    }

    header: Rectangle {
        topLeftRadius: dia.radius
        topRightRadius: dia.radius

        color: colorful ? Constants.accent : Constants.palette.dialogBg

        height: 50

        Text {
            anchors.fill: parent
            font.pixelSize: 25

            text: title
            color: Constants.palette.text

            font.bold: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    background: Rectangle {
        radius: dia.radius

        color: Constants.palette.dialogBg
    }

    Component.onCompleted: {
        footer.delegate.background.color = Constants.accent
    }

    footer: DialogButtonBox {
        id: control

        implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                                (control.count === 1 ? implicitContentWidth * 2 : implicitContentWidth) + leftPadding + rightPadding)
        implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                                 implicitContentHeight + topPadding + bottomPadding)
        contentWidth: (contentItem as ListView)?.contentWidth

        spacing: 6
        padding: 6
        alignment: count === 1 ? Qt.AlignRight : undefined

        delegate: Button {
            width: control.count === 1 ? control.availableWidth / 2 : undefined

            background: Rectangle {
                radius: 10
                color: accents.qmlColor(accents.qml(settings.accent))
            }
        }

        contentItem: ListView {
            implicitWidth: contentWidth
            model: control.contentModel
            spacing: control.spacing
            orientation: ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.SnapToItem
        }

        background: Rectangle {
            implicitHeight: 40
            x: 1; y: 1
            width: parent.width - 2
            height: parent.height - 2

            color: Constants.palette.dialogBg

            bottomLeftRadius: dia.radius
            bottomRightRadius: dia.radius
        }
    }
}
