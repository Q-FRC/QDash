import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 6.7
import QtQuick.Dialogs

import QDash.Dialogs
import QDash.Constants

AnimatedDialog {
    width: Math.min(window.width, 625)
    height: window.height

    standardButtons: Dialog.Ok | Dialog.Cancel
    title: "Accent Editor"

    onAccepted: {
        accents.save()
        // forcefully reload the current accent in case it was changed
        Constants.setAccent(settings.accent)
    }

    onRejected: accents.load()

    ColorDialog {
        id: colorDialog
        onAccepted: platformHelper.copy(colorDialog.selectedColor)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8

        RowLayout {
            uniformCellSizes: true

            Repeater {
                model: ["Name", "Accent", "Tab", "QML Accent"]

                Rectangle {
                    color: "transparent"

                    Layout.fillWidth: true
                    height: 30

                    Text {
                        anchors.fill: parent
                        color: Constants.palette.text
                        font.pixelSize: Math.round(18)

                        text: modelData

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        ListView {
            clip: true

            boundsBehavior: Flickable.StopAtBounds

            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: accents
            delegate: AccentDisplay {
                width: listView.width
            }
        }

        RowLayout {
            spacing: 15
            uniformCellSizes: true

            Button {
                font.pixelSize: 15
                Layout.fillWidth: true
                text: "Add"

                onClicked: accents.add()
            }

            Button {
                font.pixelSize: 15
                Layout.fillWidth: true
                text: "Pick Color"

                onClicked: colorDialog.open()
            }
        }
    }
}
