// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

import QtQuick.Shapes 2.15

PrimitiveWidget {
    id: widget

    property string shape: "Rectangle"

    propertyKeys: ["shape"]

    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    property string itemValue

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

    function update(value) {
        widget.connected = true
        itemValue = value
    }

    ShapeHandler {
        id: shape

        itemShape: widget.shape
        itemColor: widget.itemValue

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
                        
                    }
                }
            }
        }
    }
}
