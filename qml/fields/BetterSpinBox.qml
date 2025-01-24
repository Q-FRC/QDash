import QtQuick
import QtQuick.Controls

import QFRCDashboard

SpinBox {
    id: spin
    property bool valid: true

    from: -0x7FFFFFFF
    to: 0x7FFFFFFF

    contentItem: TextInput {
        text: parent.textFromValue(parent.value, parent.locale)

        font: parent.font
        color: Constants.palette.text
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        inputMethodHints: Qt.ImhFormattedNumbersOnly

        onEditingFinished: spin.value = parseFloat(text.replace(/,/g, ""))
    }

    up.indicator: Rectangle {
        x: spin.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: 30 * Constants.scalar
        implicitHeight: 30 * Constants.scalar
        color: Constants.palette.bg

        border.color: valid ? Constants.palette.text : "red"
        border.width: 1

        Behavior on border.color {
            ColorAnimation {
                duration: 250
            }
        }

        Text {
            text: "+"
            font.pixelSize: spin.font.pixelSize * 1.5
            color: Constants.palette.text
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    down.indicator: Rectangle {
        x: spin.mirrored ? parent.width - width : 0
        height: parent.height
        implicitWidth: 30 * Constants.scalar
        implicitHeight: 30 * Constants.scalar
        color: Constants.palette.bg

        border.color: valid ? Constants.palette.text : "red"
        border.width: 1

        Behavior on border.color {
            ColorAnimation {
                duration: 250
            }
        }

        Text {
            text: "-"
            font.pixelSize: spin.font.pixelSize * 1.5
            color: Constants.palette.text
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    background: Rectangle {
        color: Constants.palette.bg
        width: parent.width

        border.color: valid ? Constants.palette.text : "red"
        border.width: 1

        Behavior on border.color {
            ColorAnimation {
                duration: 250
            }
        }

        radius: 5 * Constants.scalar
    }
}
