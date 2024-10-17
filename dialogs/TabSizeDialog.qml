import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

Dialog {
    id: tabSizeDialog

    anchors.centerIn: parent

    property alias columnValue: columnValue
    property alias rowValue: rowValue

    function openUp(rows, cols) {
        open()
        columnValue.value = cols
        rowValue.value = rows
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Row {
            Text {
                text: "Columns:"
                font.pixelSize: 17
                color: Constants.palette.text
            }

            SpinBox {
                id: columnValue
                from: 1
                to: 99

                font.pixelSize: 17
            }
        }

        Row {
            Text {
                text: "Rows:"
                font.pixelSize: 17
                color: Constants.palette.text
            }

            SpinBox {
                id: rowValue
                from: 1
                to: 99

                font.pixelSize: 17
            }
        }

        DialogButtonBox {
            Shortcut {
                context: Qt.WidgetWithChildrenShortcut
                sequences: [Qt.Key_Escape]

                onActivated: tabSizeDialog.reject()
            }

            Button {
                text: qsTr("Ok")
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            }
            Button {
                text: qsTr("Cancel")
                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            }

            width: parent.width
            font.pixelSize: 15

            onAccepted: tabSizeDialog.accept()
            onRejected: tabSizeDialog.reject()
        }
    }
}
