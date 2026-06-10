// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PrimitiveWidget {
    id: widget

    readOnly: true
    roleString: "colorText"

    propertyKeys: ["shape"]
    menuExtension: Component {
        Menu {
            id: switchMenu

            title: "Switch Widget..."

            MenuItem {
                text: "Text"

                onTriggered: {
                    model.type = "string"
                }
            }

            MenuItem {
                text: "Text Display"

                onTriggered: {
                    model.type = "textDisplay"
                }
            }
        }
    }

    property string itemValue
    property string shape: "Rectangle"
    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    function update(value) {
        widget.connected = true
        itemValue = value
    }

    ShapeHandler {
        id: shape

        itemColor: widget.itemValue
        itemShape: widget.shape

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 10
            right: parent.right
            top: titleField.bottom
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

        LabeledSpinBox {
            bindedProperty: "titleFontSize"
            label: "Title Font Size"
        }

        SectionHeader {
            label: "Color Settings"
        }

        LabeledComboBox {
            bindedProperty: "shape"
            label: "Shape"
            model: shapeChoices
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
