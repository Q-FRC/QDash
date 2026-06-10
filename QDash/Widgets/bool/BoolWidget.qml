// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Controls
import QDash.Widgets

import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int checkboxSize: 40

    propertyKeys: ["checkboxSize"]

    menuExtension: Component {
        Menu {
            id: switchMenu
            title: "Switch Widget..."

            MenuItem {
                text: "Color Display"
                onTriggered: {
                    model.type = "color";
                }
            }
        }
    }

    function update(value) {
        connected = true;
        control.checked = value;
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        CheckBox {
            id: control

            checked: false

            enabled: widget.connected

            anchors {
                centerIn: parent
            }

            indicator.implicitHeight: checkboxSize
            indicator.implicitWidth: checkboxSize

            onToggled: widget.setValue(checked)
        }
    }

    configContent: ColumnLayout {
        id: layout
        spacing: 12
        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true

        SectionHeader {
            label: "Font Settings"
        }

        RowLayout {
            LabeledSpinBox {
                label: "Title Font Size"
                bindedProperty: "titleFontSize"
            }

            LabeledSpinBox {
                label: "Checkbox Size"
                bindedProperty: "checkboxSize"
            }
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            label: "Topic"
            bindedProperty: "item_topic"
        }
    }
}
