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

    standardButtons: Dialog.Ok | Dialog.Cancel

    Shortcut {
        onActivated: reject()
        sequence: Qt.Key_Escape
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        RowLayout {
            Layout.fillWidth: true
            uniformCellSizes: true

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

        RowLayout {
            Layout.fillWidth: true
            uniformCellSizes: true

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
    }
}
