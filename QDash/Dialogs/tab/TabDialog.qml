// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Config

NativeDialog {
    id: tabConfigDialog

    title: "Configure Tab"
    height: cols.implicitHeight + footer.height + 20
    width: 250

    property int columns
    property int rows
    property string name

    onOpened: {
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
        id: cols
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        RowLayout {
            Layout.fillWidth: true

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

            horizontalAlignment: Text.AlignHCenter

            bindTarget: tabConfigDialog
            bindedProperty: "name"
        }
    }
}
