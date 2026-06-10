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

    readOnly: true
    roleString: "color"
    typeString: "bool"
    widgetLabel: "Color"

    property color falseColor: "#FF0000"
    property color trueColor: "#00FF00"

    property bool itemValue: false

    property string shape: "Rectangle"
    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    function update(value) {
        connected = true
        itemValue = value
    }

    propertyKeys: ["falseColor", "trueColor", "shape"]

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

        RowLayout {
            Layout.fillWidth: true

            ColorField {
                bindedProperty: "trueColor"
                label: "True Color"
            }

            ColorField {
                bindedProperty: "falseColor"
                label: "False Color"
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

    ShapeHandler {
        id: shape

        itemColor: widget.itemValue ? widget.trueColor : widget.falseColor
        itemShape: widget.shape

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 10
            right: parent.right
            top: titleField.bottom
        }
    }
}
