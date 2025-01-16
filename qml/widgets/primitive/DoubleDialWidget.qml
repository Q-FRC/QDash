import QtQuick
import QtQuick.Controls 6.6
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget
    property string item_topic

    property int item_fontSize: 15
    property double item_stepSize: 0.1

    property double item_startAngle: -180
    property double item_endAngle: 180

    property double item_lowerBound: 0
    property double item_upperBound: 100000.0

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Spin Box"
            onTriggered: {
                model.type = "double"
            }
        }
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    DoubleSpinBox {
        id: spin

        font.pixelSize: item_fontSize * Constants.scalar

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                value = ntValue
                dial.value = ntValue
            }
        }

        value: 0
        stepSize: item_stepSize

        from: item_lowerBound
        to: item_upperBound

        anchors {
            top: dial.bottom
            topMargin: 8

            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
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

        onValueModified: {
            dial.value = value
            topicStore.setValue(item_topic, value)
        }

        function setValue(val) {
            value = val
            topicStore.setValue(item_topic, value)
        }
    }

    Dial {
        id: dial

        background: Rectangle {
            x: dial.width / 2 - width / 2
            y: dial.height / 2 - height / 2

            implicitWidth: 140
            implicitHeight: 140

            width: Math.max(64, Math.min(dial.width, dial.height))
            height: width

            color: "transparent"
            radius: width / 2

            border.color: Constants.accent
            opacity: dial.enabled ? 1 : 0.3
        }

        handle: Rectangle {
            id: handleItem
            x: dial.background.x + dial.background.width / 2 - width / 2
            y: dial.background.y + dial.background.height / 2 - height / 2

            width: Math.min(parent.width, parent.height) / 5
            height: Math.min(parent.width, parent.height) / 5

            color: Constants.accent
            radius: 8 * Constants.scalar

            antialiasing: true
            opacity: dial.enabled ? 1 : 0.3

            transform: [
                Translate {
                    y: -Math.min(
                           dial.background.width,
                           dial.background.height) * 0.4 + handleItem.height / 2
                },
                Rotation {
                    angle: dial.angle
                    origin.x: handleItem.width / 2
                    origin.y: handleItem.height / 2
                }
            ]
        }

        inputMode: Dial.Circular

        font.pixelSize: item_fontSize * Constants.scalar

        value: 0
        stepSize: item_stepSize

        height: parent.height / 3
        width: parent.width / 3

        from: item_lowerBound
        to: item_upperBound

        startAngle: item_startAngle
        endAngle: item_endAngle

        anchors {
            top: titleField.bottom
            horizontalCenter: parent.horizontalCenter

            topMargin: 10
        }

        onMoved: {
            spin.setValue(value)
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic

        spin.value = topicStore.getValue(item_topic)
        dial.value = topicStore.getValue(item_topic)
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()

            upField.open()
            lowField.open()
            stepField.open()
            startField.open()
            endField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fontField.accept()
            upField.accept()
            lowField.accept()
            stepField.accept()
            startField.accept()
            endField.accept()
        }

        ScrollView {
            id: scroll
            contentWidth: width - 5 * Constants.scalar - effectiveScrollBarWidth

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: -20

                rightMargin: 5
            }

            ColumnLayout {
                id: layout
                spacing: 25 * Constants.scalar

                anchors.fill: parent
                anchors.leftMargin: 2

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

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: fontField

                        label: "Font Size"

                        bindedProperty: "item_fontSize"
                        bindTarget: widget
                    }
                }

                SectionHeader {
                    label: "Spin Box Settings"
                }

                RowLayout {
                    uniformCellSizes: true

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: lowField

                        label: "Lower Bound"

                        bindedProperty: "item_lowerBound"
                        bindTarget: widget
                    }

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: upField

                        label: "Upper Bound"

                        bindedProperty: "item_upperBound"
                        bindTarget: widget
                    }
                }

                LabeledDoubleSpinBox {
                    Layout.fillWidth: true

                    id: stepField

                    label: "Step Size"

                    bindedProperty: "item_stepSize"
                    bindTarget: widget

                    from: 0
                    stepSize: 0.1
                }

                SectionHeader {
                    label: "Dial Settings"
                }

                RowLayout {
                    uniformCellSizes: true

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: startField

                        label: "Start Angle"

                        bindedProperty: "item_startAngle"
                        bindTarget: widget
                    }

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: endField

                        label: "End Angle"

                        bindedProperty: "item_endAngle"
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
