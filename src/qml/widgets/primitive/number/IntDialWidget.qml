import QtQuick
import QtQuick.Controls 6.6
import QtQuick.Layouts 6.6
 
import fields
import items
import config
import basewidget
import constants


PrimitiveWidget {
    id: widget

    property int item_fontSize: 20

    property double item_startAngle: 180
    property double item_endAngle: 540

    property int item_stepSize: 1

    property int item_lowerBound: -1000
    property int item_upperBound: 1000

    BetterMenu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Spin Box"
            onTriggered: {
                model.type = "int"
            }
        }

        MenuItem {
            text: "Radial Gauge"
            onTriggered: {
                model.type = "gauge"
            }
        }

        MenuItem {
            text: "Number Display"
            onTriggered: {
                model.type = "intDisplay"
            }
        }
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    function update(value) {
        spin.value = value
        dial.value = value
    }

    BetterSpinBox {
        id: spin

        font.pixelSize: item_fontSize * Constants.scalar

        value: 0

        from: item_lowerBound
        to: item_upperBound
        stepSize: item_stepSize

        valid: widget.valid
        connected: widget.connected

        anchors {
            bottom: parent.bottom
            bottomMargin: parent.height / 10

            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        onValueModified: {
            dial.value = value
            widget.setValue(value)
        }

        function move(val) {
            let previousValue = value

            value = val
            widget.setValue(value)

            widget.valid = Math.abs(previousValue - value) < 0.01
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
            opacity: widget.connected ? 1 : 0.3
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
            opacity: widget.connected ? 1 : 0.3

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

        enabled: widget.connected

        anchors {
            top: titleField.bottom
            bottom: spin.top
            horizontalCenter: parent.horizontalCenter

            margins: 10
        }

        onMoved: spin.move(parseInt(value))
    }

    // TODO: Remove these IDs. We don't need them anymore (besides config)
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

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: lowField

                    label: "Lower Bound"

                    bindedProperty: "item_lowerBound"
                    bindTarget: widget
                }

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: upField

                    label: "Upper Bound"

                    bindedProperty: "item_upperBound"
                    bindTarget: widget
                }
            }

            LabeledSpinBox {
                Layout.fillWidth: true

                id: stepField

                label: "Step Size"

                bindedProperty: "item_stepSize"
                bindTarget: widget

                from: 0
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
