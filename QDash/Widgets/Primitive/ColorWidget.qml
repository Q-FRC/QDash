// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

import QtQuick.Shapes 2.15

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

    function setColor() {
        shape.itemColor = widget.itemValue ? widget.trueColor : widget.falseColor
        shape.itemShape = widget.shape
        shape.setColor()
    }

    function update(value) {
        connected = true
        itemValue = value
        setColor()
    }

    onFalseColorChanged: setColor()
    onTrueColorChanged: setColor()
    onShapeChanged: setColor()

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
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        id: titleFontField

                        label: "Title Font Size"

                        bindedProperty: "titleFontSize"
                        bindTarget: widget
                    }

                    SectionHeader {
                        label: "Color Settings"
                    }

                    LabeledComboBox {
                        id: shapeField

                        Layout.fillWidth: true
                        model: shapeChoices

                        label: "Shape"

                        bindedProperty: "shape"
                        bindTarget: widget
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        ColorField {
                            id: trueField

                            Layout.fillWidth: true

                            label: "True Color"

                            bindedProperty: "trueColor"
                            bindTarget: widget
                        }

                        ColorField {
                            id: falseField

                            Layout.fillWidth: true

                            label: "False Color"

                            bindedProperty: "falseColor"
                            bindTarget: widget
                        }
                    }

                    SectionHeader {
                        label: "NT Settings"
                    }

                    LabeledTextField {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        id: topicField

                        label: "Topic"

                        bindedProperty: "item_topic"
                        bindTarget: widget
                    }
                }
            }
        }
    }
}
