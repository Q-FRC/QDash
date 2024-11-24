import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QtMultimedia

import QFRCDashboard

SpinBox {
    id: spin
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget

    font.pixelSize: 18

    from: 0
    to: 1E9

    contentItem: TextInput {
        text: parent.textFromValue(parent.value, parent.locale)

        font: parent.font
        color: Constants.palette.text
        // selectionColor: "#21be2b"
        // selectedTextColor: "#ffffff"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        inputMethodHints: Qt.ImhFormattedNumbersOnly
    }

    function open() {
        value = bindTarget[bindedProperty]
    }

    function accept() {
        bindTarget[bindedProperty] = value
    }

    up.indicator: Rectangle {
        x: spin.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40
        color: Constants.palette.bg
        border.color: Constants.palette.text

        Text {
            text: "+"
            font.pixelSize: spin.font.pixelSize * 2
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
        implicitWidth: 40
        implicitHeight: 40
        color: Constants.palette.bg
        border.color: Constants.palette.text

        Text {
            text: "-"
            font.pixelSize: spin.font.pixelSize * 2
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
        border.color: Constants.palette.text
        border.width: 2
        radius: 5
    }

    Text {
        id: floatingLabel
        text: label
        color: Constants.palette.text

        font.pixelSize: 15

        anchors {
            left: spin.left
            bottom: spin.top

            bottomMargin: -2
            leftMargin: 10
        }
    }
}
