import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import fields
import items
import config
import basewidget
import constants

import QtQuick.Shapes 2.15

PrimitiveWidget {
    id: widget

    // @disable-check M311
    property var item_shape: "Rectangle"

    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    property string itemValue

    BetterMenu {
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

    function setColor() {
        shape.itemColor = itemValue
        shape.itemShape = item_shape
        shape.setColor()
    }

    function update(value) {
        itemValue = value
        setColor()
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    onItem_shapeChanged: setColor()

    ShapeHandler {
        id: shape

        itemShape: item_shape

        anchors {
            top: titleField.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            margins: 10
        }
    }

    BaseConfigDialog {
        id: config

        content: ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar
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

                bindedProperty: "item_titleFontSize"
                bindTarget: widget
            }

            SectionHeader {
                label: "Color Settings"
            }

            LabeledComboBox {
                id: shapeField

                Layout.fillWidth: true
                choices: shapeChoices

                label: "Shape"

                bindedProperty: "item_shape"
                bindTarget: widget
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
