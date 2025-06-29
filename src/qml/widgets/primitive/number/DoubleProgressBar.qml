import QtQuick 2.15
import QtQuick.Controls.Material
import QtQuick.Layouts
 
import fields
import items
import config
import basewidget
import constants


PrimitiveWidget {
    id: widget

    property int item_fontSize: 20

    property int item_numTicks: 5

    property string item_suffix: ""

    property double item_lowerBound: 0.0
    property double item_upperBound: 100.0

    property bool item_vertical: false

    BetterMenu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Spin Box"
            onTriggered: {
                model.type = "double"
            }
        }

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

    function update(value) {
        bar.value = value
    }

    ProgressBar {
        id: bar

        font.pixelSize: item_fontSize * Constants.scalar

        from: item_lowerBound
        to: item_upperBound

        rotation: item_vertical ? -90 : 0

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: titleField.height / 2
        }

        width: item_vertical ? (parent.height - titleField.height - 50
                                * Constants.scalar) : (parent.width - 50 * Constants.scalar)

        Repeater {
            model: item_numTicks + 1

            Item {
                anchors {
                    top: bar.bottom
                }

                x: (bar.width * index) / item_numTicks - Constants.scalar

                Rectangle {
                    id: tick
                    width: 2 * Constants.scalar
                    height: 10 * Constants.scalar

                    color: Constants.palette.text
                }

                Text {
                    color: Constants.palette.text

                    text: (bar.from + index * (bar.to - bar.from) / item_numTicks).toFixed(
                              1) + item_suffix

                    font.pixelSize: 15 * Constants.scalar

                    anchors {
                        top: tick.bottom
                        topMargin: 15 * Constants.scalar

                        horizontalCenter: tick.horizontalCenter
                    }

                    rotation: item_vertical ? 90 : 0
                }
            }
        }

        Text {
            id: txt

            color: Constants.palette.text
            font.pixelSize: item_fontSize * Constants.scalar

            text: parent.value + item_suffix
            rotation: item_vertical ? 90 : 0

            anchors {
                bottom: bar.top
                bottomMargin: 25 * Constants.scalar

                horizontalCenter: parent.horizontalCenter
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

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: fontField

                    label: "Font Size"

                    bindedProperty: "item_fontSize"
                    bindTarget: widget
                }
            }

            SectionHeader {
                label: "Bar Settings"
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

            RowLayout {
                uniformCellSizes: true

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: tickField

                    label: "Number of Ticks"

                    bindedProperty: "item_numTicks"
                    bindTarget: widget

                    stepSize: 1
                }

                LabeledTextField {
                    Layout.fillWidth: true

                    id: suffixField

                    label: "Suffix"

                    bindedProperty: "item_suffix"
                    bindTarget: widget
                }
            }

            LabeledCheckbox {
                Layout.fillWidth: true

                id: vertField

                label: "Vertical?"

                bindedProperty: "item_vertical"
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
