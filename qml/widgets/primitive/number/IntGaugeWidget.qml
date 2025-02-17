import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property int item_fontSize: 20

    property int item_ticks: 15

    property double item_startAngle: -135
    property double item_endAngle: 135

    property int item_min: 0
    property int item_max: 100

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Dial"
            onTriggered: {
                model.type = "dial"
            }
        }

        MenuItem {
            text: "Spin Box"
            onTriggered: {
                model.type = "int"
            }
        }

        MenuItem {
            text: "Number Display"
            onTriggered: {
                model.type = "intDisplay"
            }
        }
    }

    function fixSize() {
        gauge.width = parent.width
        gauge.height = parent.height - titleField.height
        gauge.fixSize()
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)

        fixSize()
    }

    onHeightChanged: {
        fixSize()
    }

    onWidthChanged: {
        fixSize()
    }

    RadialGauge {
        id: gauge

        valueFontSize: item_fontSize * Constants.scalar

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                value = ntValue
            }
        }

        value: 0

        numTicks: item_ticks

        minValue: item_min
        maxValue: item_max

        startAngle: item_startAngle
        endAngle: item_endAngle

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
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic
        gauge.value = topicStore.getValue(item_topic)
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

            tickField.open()

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

            tickField.accept()
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
                    label: "Gauge Settings"
                }

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: tickField

                    label: "Number of Ticks"

                    bindedProperty: "item_ticks"
                    bindTarget: widget
                }

                RowLayout {
                    uniformCellSizes: true

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: lowField

                        label: "Minimum Value"

                        bindedProperty: "item_min"
                        bindTarget: widget
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: upField

                        label: "Maximum Value"

                        bindedProperty: "item_max"
                        bindTarget: widget
                    }
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
