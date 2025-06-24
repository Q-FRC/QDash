import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

import fields
import items
import config
import basewidget
import constants

import misc

PrimitiveWidget {
    id: widget

    property string item_topic

    property list<bool> values

    function valueChanged() {
        setValue(values)
    }

    function update(value) {
        values = value
    }

    Item {
        anchors {
            top: titleField.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Repeater {
            model: 6
            Shape {
                id: shape

                height: Math.min(parent.height, parent.width) / 2.3
                width: height * 1.15

                rotation: -60 * modelData
                transformOrigin: Item.Top

                anchors.centerIn: parent
                anchors.verticalCenterOffset: height / 2

                MaskedMouseArea {
                    id: leftTri
                    anchors.fill: parent
                    isLeft: true

                    onClicked: {
                        values[modelData * 2] = !values[modelData * 2]
                        valueChanged()
                    }
                }

                MaskedMouseArea {
                    id: rightTri
                    anchors.fill: parent
                    isLeft: false

                    onClicked: {
                        values[modelData * 2 + 1] = !values[modelData * 2 + 1]
                        valueChanged()
                    }
                }

                ShapePath {
                    strokeWidth: 2
                    strokeColor: "black"
                    fillColor: values[modelData * 2 + 1] ? "green" : "red"

                    startX: shape.width
                    startY: shape.height

                    PathLine {
                        x: shape.width / 2
                        y: shape.height
                    }
                    PathLine {
                        x: shape.width / 2
                        y: 0
                    }

                    PathLine {
                        x: shape.width
                        y: shape.height
                    }
                }
                ShapePath {
                    strokeWidth: 2
                    strokeColor: "black"
                    fillColor: values[modelData * 2] ? "green" : "red"

                    startX: 0
                    startY: shape.height

                    PathLine {
                        x: shape.width / 2
                        y: shape.height
                    }
                    PathLine {
                        x: shape.width / 2
                        y: 0
                    }

                    PathLine {
                        x: 0
                        y: shape.height
                    }
                }
            }
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

            RowLayout {
                uniformCellSizes: true

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: titleFontField

                    label: "Title Font Size"

                    bindedProperty: "item_titleFontSize"
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
