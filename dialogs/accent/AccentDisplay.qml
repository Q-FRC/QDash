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

    function getQml() {
        return model.qml
    }

    function setQml(newQml) {
        model.qml = newQml
    }

    RowLayout {
        anchors {
            left: parent.left
            right: parent.right

            margins: 5
        }

        width: parent.width
        uniformCellSizes: true
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

        ComboBox {
            Layout.fillWidth: true

            Component.onCompleted: currentIndex = indexOfValue(getQml())

            id: qml
            model: [
                "Lime",
                "Green",
                "Emerald",
                "Teal",
                "Cyan",
                "Cobalt",
                "Indigo",
                "Violet",
                "Pink",
                "Magenta",
                "Crimson",
                "Red",
                "Orange",
                "Amber",
                "Yellow",
                "Brown",
                "Olive",
                "Steel",
                "Mauve",
                "Taupe",
            ]

            font.pixelSize: 15

            onActivated: (index) => setQml(currentText)
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

        Rectangle {
            Layout.fillWidth: true

            height: 40

            color: accents.qmlColor(qml.currentText)
        }
    }

}
