import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Shapes 2.15

import QFRCDashboard

PrimitiveWidget {
    id: widget

    property color item_falseColor: "#FF0000"
    property color item_trueColor: "#00FF00"

    // @disable-check M311
    property var item_shape: "Rectangle"

    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    property bool itemValue

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

    function setColor() {
        shape.itemColor = itemValue ? item_trueColor : item_falseColor
        shape.itemShape = item_shape
        shape.setColor()
    }

    function update(value) {
        itemValue = value
        setColor()
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    onItem_falseColorChanged: setColor()
    onItem_trueColorChanged: setColor()
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

        function openDialog() {
            topicField.open()
            titleFontField.open()
            shapeField.open()
            trueField.open()
            falseField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            shapeField.accept()
            trueField.accept()
            falseField.accept()
        }

        ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: 5 * Constants.scalar

                rightMargin: 5
            }

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

            RowLayout {
                Layout.fillWidth: true

                ColorField {
                    id: trueField

                    Layout.fillWidth: true

                    label: "True Color"

                    bindedProperty: "item_trueColor"
                    bindTarget: widget
                }

                ColorField {
                    id: falseField

                    Layout.fillWidth: true

                    label: "False Color"

                    bindedProperty: "item_falseColor"
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
