// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 6.8

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Controls

Loader {
    id: loader
    active: false
    asynchronous: true
    onLoaded: {
        item.columns = columns
        item.rows = rows
        item.name = name
        item.open()
    }

    property int columns
    property int rows
    property string name

    sourceComponent: active ? src : undefined

    signal accepted

    function open() {
        active = true
    }

    property Component src: CarboxylDialog {
        title: "Configure Tab"

        implicitWidth: 250

        popupType: Popup.Window

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

            loader.accepted()
        }

        onClosed: loader.active = false

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

                    bindTarget: loader
                    bindedProperty: "columns"
                }

                LabeledSpinBox {
                    Layout.fillWidth: true
                    id: rowValue
                    from: 1
                    to: 99

                    label: "Rows"

                    bindTarget: loader
                    bindedProperty: "rows"
                }
            }

            LabeledTextField {
                id: nameValue
                Layout.fillWidth: true

                label: "Tab Name"

                horizontalAlignment: Text.AlignHCenter

                bindTarget: loader
                bindedProperty: "name"
            }
        }
    }
}
