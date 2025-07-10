import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts
 
import fields
import items
import config
import basewidget
import constants


PrimitiveWidget {
    id: widget

    property double item_stepSize: 0.1
    property int item_fontSize: 20

    property double item_lowerBound: -100000.0
    property double item_upperBound: 100000.0

    BetterMenu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Dial"
            onTriggered: {
                model.type = "doubleDial"
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

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    function update(value) {
        spin.value = value
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        BetterDoubleSpinBox {
            id: spin

            font.pixelSize: item_fontSize * Constants.scalar

            valid: widget.valid
            connected: widget.connected

            value: 0
            from: item_lowerBound
            to: item_upperBound
            stepSize: item_stepSize

            anchors {
                verticalCenter: parent.verticalCenter

                left: parent.left
                right: parent.right
            }

            onValueModified: widget.setValue(value)
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
