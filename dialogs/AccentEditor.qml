import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.7
import QtQuick.Dialogs

import QFRCDashboard

Dialog {
    anchors.centerIn: parent

    width: parent.width / 1.5
    height: parent.height / 1.1

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        accents.save()
        // forcefully reload the current accent in case it was changed
        Constants.setAccent(settings.accent)
    }

    onRejected: accents.load()

    ColorDialog {
        id: colorDialog
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        Text {
            text: "Edit Accents"

            font.pixelSize: 20
            font.bold: true

            color: "#FFFFFF"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop

            Layout.fillWidth: true
        }

        RowLayout {
            uniformCellSizes: true

            Repeater {
                model: ["Name", "Accent", "Tab"]

                Rectangle {
                    color: "transparent"
                    border {
                        color: "white"
                        width: 2
                    }

                    Layout.fillWidth: true
                    height: 30

                    Text {
                        anchors.fill: parent
                        color: "white"
                        font.pixelSize: 18

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

        Row {
            Button {
                text: "Add"

                onClicked: accents.add()
            }

            Button {
                text: "Pick Color"

                function setColor() {
                    accents.copy(colorDialog.selectedColor)
                    colorDialog.accepted.disconnect(setColor)
                }

                onClicked: {
                    colorDialog.accepted.connect(setColor)
                    colorDialog.open()
                }
            }
        }
    }
}
