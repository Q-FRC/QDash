// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Controls
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 6.8

Loader {
    id: loader

    property int columns
    property string name
    property int rows
    property Component src: CarboxylDialog {
        property int columns
        property string name
        property int rows

        implicitWidth: 250
        popupType: Popup.Window
        standardButtons: Dialog.Ok | Dialog.Cancel
        title: "Configure Tab"

        onAccepted: {
            columnValue.accept()
            rowValue.accept()
            nameValue.accept()

            loader.accepted()
        }
        onClosed: loader.active = false
        onOpened: {
            columnValue.open()
            rowValue.open()
            nameValue.open()
        }

        Shortcut {
            sequence: Qt.Key_Escape

            onActivated: reject()
        }

        ColumnLayout {
            id: cols

            anchors.fill: parent
            anchors.margins: 10
            spacing: 15

            RowLayout {
                Layout.fillWidth: true

                LabeledSpinBox {
                    id: columnValue

                    Layout.fillWidth: true
                    bindTarget: loader
                    bindedProperty: "columns"
                    from: 1
                    label: "Columns"
                    to: 99
                }

                LabeledSpinBox {
                    id: rowValue

                    Layout.fillWidth: true
                    bindTarget: loader
                    bindedProperty: "rows"
                    from: 1
                    label: "Rows"
                    to: 99
                }
            }

            LabeledTextField {
                id: nameValue

                Layout.fillWidth: true
                bindTarget: loader
                bindedProperty: "name"
                horizontalAlignment: Text.AlignHCenter
                label: "Tab Name"
            }
        }
    }

    signal accepted

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: {
        item.columns = columns
        item.rows = rows
        item.name = name
        item.open()
    }
}
