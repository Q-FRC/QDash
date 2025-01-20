import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property list<bool> values

    function updateTopic(ntTopic, ntValue) {
        if (ntTopic === item_topic) {
            values = ntValue
        }
    }

    Component.onCompleted: {
        topicStore.topicUpdate.connect(updateTopic)

        item_topic = model.topic
    }

    Component.onDestruction: {
        if (topicStore !== null) {
            topicStore.topicUpdate.disconnect(updateTopic)
            topicStore.unsubscribe(item_topic)
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic

        values = topicStore.getValue(item_topic)
    }

    Repeater {
        model: 6
        Shape {
            id: shape

            height: Math.min(parent.height, parent.width) / 3
            width: height * 1.15

            rotation: -60 * modelData
            transformOrigin: Item.Top

            anchors.centerIn: parent
            anchors.verticalCenterOffset: height / 2

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

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
        }

        ColumnLayout {
            id: layout
            spacing: 25 * Constants.scalar

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: -20

                rightMargin: 5
            }

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
