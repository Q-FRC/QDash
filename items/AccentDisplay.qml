import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.7

Rectangle {
    clip: true

    height: 100
    border {
        color: "white"
        width: 2
    }
    color: "transparent"

    RowLayout {
        anchors {
            left: parent.left
            right: parent.right

            margins: 5
        }

        width: parent.width
        height: 50

        TextField {
            Layout.fillWidth: true

            id: name
            text: model.name

            onTextEdited: model.name = text

            font.pixelSize: 15
        }

        TextField {
            Layout.fillWidth: true

            id: accent
            text: model.accent

            onTextEdited: model.accent = text

            font.pixelSize: 15

            validator: RegularExpressionValidator {
                regularExpression: /^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/
            }
        }

        TextField {
            Layout.fillWidth: true

            id: light
            text: model.light

            onTextEdited: model.light = text

            font.pixelSize: 15

            validator: RegularExpressionValidator {
                regularExpression: /^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/
            }
        }
    }

    RowLayout {
        id: preview
        uniformCellSizes: true
        y: 50

        anchors {
            left: parent.left
            right: parent.right

            margins: 8
        }

        height: 50

        Button {
            text: "Remove"

            onClicked: accents.remove(model.idx)
        }

        Rectangle {
            Layout.fillWidth: true
            // Layout.fillHeight: true
            height: 40

            color: accent.text
        }

        Rectangle {
            Layout.fillWidth: true
            // Layout.fillHeight: true
            height: 40

            color: light.text
        }
    }

}
