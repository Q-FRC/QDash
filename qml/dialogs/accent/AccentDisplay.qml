import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.7

import QFRCDashboard

Rectangle {
    clip: true

    height: 105 * Constants.scalar
    color: "transparent"

    function getQml() {
        return model.qml
    }

    function setQml(newQml) {
        model.qml = newQml
    }

    RowLayout {
        id: fields
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right

            margins: 5 * Constants.scalar
        }

        width: parent.width
        uniformCellSizes: true

        BetterTextField {
            Layout.fillWidth: true

            id: name
            text: model.name

            onTextEdited: model.name = text

            font.pixelSize: Math.round(15 * Constants.scalar)
        }

        BetterTextField {
            Layout.fillWidth: true

            id: accent
            text: model.accent

            onTextEdited: model.accent = text

            font.pixelSize: Math.round(15 * Constants.scalar)

            validator: RegularExpressionValidator {
                regularExpression: /^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/
            }
        }

        BetterTextField {
            Layout.fillWidth: true

            id: light
            text: model.light

            onTextEdited: model.light = text

            font.pixelSize: Math.round(15 * Constants.scalar)

            validator: RegularExpressionValidator {
                regularExpression: /^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/
            }
        }

        BetterComboBox {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: currentIndex = indexOfValue(getQml())

            id: qml
            model: ["Lime", "Green", "Emerald", "Teal", "Cyan", "Cobalt", "Indigo", "Violet", "Pink", "Magenta", "Crimson", "Red", "Orange", "Amber", "Yellow", "Brown", "Olive", "Steel", "Mauve", "Taupe"]

            font.pixelSize: Math.round(16 * Constants.scalar)

            onActivated: index => setQml(currentText)
        }
    }

    RowLayout {
        id: preview
        uniformCellSizes: true

        anchors {
            left: parent.left
            right: parent.right

            top: fields.bottom

            margins: 8 * Constants.scalar
            topMargin: 2
        }

        Button {
            font.pixelSize: 18 * Constants.scalar
            text: "Remove"

            onClicked: accents.remove(model.idx)

            Layout.alignment: Qt.AlignCenter
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 40 * Constants.scalar

            color: accent.text
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 40 * Constants.scalar

            color: light.text
        }

        Rectangle {
            Layout.fillWidth: true

            implicitHeight: 40 * Constants.scalar

            color: accents.qmlColor(qml.currentText)
        }
    }
}
