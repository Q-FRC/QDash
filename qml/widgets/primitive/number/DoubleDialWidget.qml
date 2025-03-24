import QtQuick
import QtQuick.Controls 6.6
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget
    property string item_topic

    property int item_fontSize: 20

    property double item_startAngle: -180
    property double item_endAngle: 180

    property double item_lowerBound: -100000.0
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

        MenuItem {
            text: "Radial Gauge"
            onTriggered: {
                model.type = "doubleGauge"
            }
        }

        MenuItem {
            text: "Progress Bar"
            onTriggered: {
                model.type = "doubleBar"
            }
        }

        MenuItem {
            text: "Number Display"
            onTriggered: {
                model.type = "doubleDisplay"
            }
        }

        MenuItem {
            text: "Match Time"
            onTriggered: {
                model.type = "matchTime"
            }
        }
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    BetterDoubleSpinBox {
        id: spin

        font.pixelSize: item_fontSize * Constants.scalar

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                value = ntValue
                dial.value = ntValue
                valid = true
            }
        }

        value: 0

        from: item_lowerBound
        to: item_upperBound

        anchors {
            bottom: parent.bottom
            bottomMargin: parent.height / 10

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
            valid = false
            topicStore.setValue(item_topic, value)
        }

        function move(val) {
            valid = Math.abs(val - value) < 0.01
            value = val
            topicStore.setValue(item_topic, value)
        }
    }

    // TODO: improve look
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
            radius: width / 2

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
            bottom: spin.top
            horizontalCenter: parent.horizontalCenter

            margins: 10
        }

        onMoved: {
            spin.move(value)
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic

        topicStore.forceUpdate(item_topic)
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()

            upField.open()
            lowField.open()
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

                topMargin: 5 * Constants.scalar

                rightMargin: 5
            }

            ColumnLayout {
                id: layout
                spacing: 12 * Constants.scalar

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
