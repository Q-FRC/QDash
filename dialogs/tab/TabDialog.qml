import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

AnimatedDialog {
    id: tabConfigDialog

    title: "Configure Tab"
    height: 270
    width: 300

    property int columns
    property int rows
    property string name

    function openUp(rows, cols, name) {
        open()
        this.columns = cols
        this.rows = rows
        this.name = name

        columnValue.open()
        rowValue.open()
        nameValue.open()
    }

    onAccepted: {
        columnValue.accept()
        rowValue.accept()
        nameValue.accept()
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

            LabeledSpinBox {
                Layout.fillWidth: true
                id: columnValue
                from: 1
                to: 99

                label: "Columns"

                bindTarget: tabConfigDialog
                bindedProperty: "columns"
            }

            LabeledSpinBox {
                Layout.fillWidth: true
                id: rowValue
                from: 1
                to: 99

                label: "Rows"

                bindTarget: tabConfigDialog
                bindedProperty: "rows"
            }
        }

        LabeledTextField {
            id: nameValue
            Layout.fillWidth: true

            label: "Tab Name"

            bindTarget: tabConfigDialog
            bindedProperty: "name"
        }
    }
}
