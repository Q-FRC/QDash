// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

PrimitiveWidget {
    id: widget

    readOnly: false
    roleString: "bool"

    propertyKeys: ["checkboxSize"]
    menuExtension: Component {
        Menu {
            id: switchMenu

            title: "Switch Widget..."

            MenuItem {
                text: "Color Display"

                onTriggered: {
                    model.type = "color"
                }
            }
        }
    }

    property int checkboxSize: 40

    function update(value) {
        connected = true
        control.checked = value
    }

    Item {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: titleField.bottom

            leftMargin: 10
            rightMargin: 10
        }

        CheckBox {
            id: control

            checked: false
            enabled: widget.connected

            indicator.implicitHeight: checkboxSize
            indicator.implicitWidth: checkboxSize

            onToggled: widget.setValue(checked)

            anchors {
                centerIn: parent
            }
        }
    }

    configContent: ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true
        spacing: 12

        SectionHeader {
            label: "Font Settings"
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "titleFontSize"
                label: "Title Font Size"
            }

            LabeledSpinBox {
                bindedProperty: "checkboxSize"
                label: "Checkbox Size"
            }
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
        }
    }
}
