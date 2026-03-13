// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int fontSize: 20

    property int ticks: 15

    property double startAngle: -135
    property double endAngle: 135

    property double min: 0
    property double max: 100

    propertyKeys: ["fontSize", "ticks", "startAngle", "endAngle", "min", "max"]

    menuExtension: Component {
        Menu {
            id: switchMenu
            title: "Switch Widget..."

            MenuItem {
                text: "Dial"
                onTriggered: {
                    model.type = "doubleDial"
                }
            }

            MenuItem {
                text: "Spin Box"
                onTriggered: {
                    model.type = "double"
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

            MenuItem {
                text: "Phase Display"
                onTriggered: {
                    model.type = "phaseShift"
                }
            }
        }
    }

    function fixSize() {
        gauge.width = width
        gauge.height = height - titleField.height
        gauge.fixSize()
    }

    Component.onCompleted: fixSize()
    onHeightChanged: fixSize()
    onWidthChanged: fixSize()

    function update(value) {
        widget.connected = true
        gauge.value = value
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10

            topMargin: 4
        }

        enabled: widget.connected

        // TODO: Fix clipping
        RadialGauge {
            id: gauge

            valueFontSize: fontSize

            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
            }

            value: 0

            numTicks: ticks

            minValue: min
            maxValue: max

            startAngle: startAngle
            endAngle: endAngle
        }
    }

    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: BaseConfigDialog {
            id: config

            content: ColumnLayout {
                id: layout
                spacing: 12
                anchors.fill: parent
                anchors.leftMargin: 2
                clip: true

                SectionHeader {
                    label: "Font Settings"
                }

                RowLayout {

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: titleFontField

                        label: "Title Font Size"

                        bindedProperty: "titleFontSize"
                        bindTarget: widget
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: fontField

                        label: "Font Size"

                        bindedProperty: "fontSize"
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

                    bindedProperty: "ticks"
                    bindTarget: widget
                }

                RowLayout {

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: lowField

                        label: "Minimum Value"

                        bindedProperty: "min"
                        bindTarget: widget
                    }

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: upField

                        label: "Maximum Value"

                        bindedProperty: "max"
                        bindTarget: widget
                    }
                }

                RowLayout {

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: startField

                        label: "Start Angle"

                        bindedProperty: "startAngle"
                        bindTarget: widget
                    }

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        id: endField

                        label: "End Angle"

                        bindedProperty: "endAngle"
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
