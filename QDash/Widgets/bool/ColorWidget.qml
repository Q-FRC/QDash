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

    property color falseColor: "#FF0000"
    property color trueColor: "#00FF00"

    property string shape: "Rectangle"

    propertyKeys: ["falseColor", "trueColor", "shape"]

    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    property bool itemValue: false

    menuExtension: Component {
        Menu {
            id: switchMenu
            title: "Switch Widget..."

            MenuItem {
                text: "Checkbox"
                onTriggered: {
                    model.type = "bool"
                }
            }
        }
    }

    function update(value) {
        connected = true
        itemValue = value
    }

    ShapeHandler {
        id: shape

        itemShape: widget.shape
        itemColor: widget.itemValue ? widget.trueColor : widget.falseColor

        anchors {
            top: titleField.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            margins: 10
        }
    }

    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: Component {
            BaseConfigDialog {
                id: config

                content: ColumnLayout {
                    id: layout
                    spacing: 12
                    anchors.fill: parent
                    anchors.leftMargin: 2
                    clip: true

                    SectionHeader {
                        label: "Font Settings"
                    }

                    LabeledSpinBox {
                        label: "Title Font Size"
                        bindedProperty: "titleFontSize"
                    }

                    SectionHeader {
                        label: "Color Settings"
                    }

                    LabeledComboBox {
                        label: "Shape"
                        bindedProperty: "shape"
                        model: shapeChoices
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        ColorField {
                            label: "True Color"
                            bindedProperty: "trueColor"
                        }

                        ColorField {
                            label: "False Color"
                            bindedProperty: "falseColor"
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
        }
    }
}
