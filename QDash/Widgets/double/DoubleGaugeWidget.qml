// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Core
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

PrimitiveWidget {
    id: widget

    property double endAngle: 135
    property int fontSize: QDashSettings.defaultFontSize
    property double max: 100
    property double min: 0
    property double startAngle: -135
    property int ticks: 15

    function fixGaugeSize() {
        gauge.width = width
        gauge.height = height - titleField.height
        gauge.fixGaugeSize()
    }

    function update(value) {
        widget.connected = true
        gauge.value = value
    }

    propertyKeys: ["fontSize", "ticks", "startAngle", "endAngle", "min", "max"]

    configContent: ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true
        spacing: 12

        SectionHeader {
            label: "Font Settings"
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "titleFontSize"
                label: "Title Font Size"
            }

            LabeledSpinBox {
                bindedProperty: "fontSize"
                label: "Font Size"
            }
        }

        SectionHeader {
            label: "Gauge Settings"
        }

        LabeledSpinBox {
            bindedProperty: "ticks"
            label: "Number of Ticks"
        }

        RowLayout {
            LabeledDoubleSpinBox {
                bindedProperty: "min"
                label: "Minimum Value"
            }

            LabeledDoubleSpinBox {
                bindedProperty: "max"
                label: "Maximum Value"
            }
        }

        RowLayout {
            LabeledDoubleSpinBox {
                bindedProperty: "startAngle"
                label: "Start Angle"
            }

            LabeledDoubleSpinBox {
                bindedProperty: "endAngle"
                label: "End Angle"
            }
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
        }
    }
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

    Component.onCompleted: fixGaugeSize()
    onHeightChanged: fixGaugeSize()
    onWidthChanged: fixGaugeSize()

    Item {
        enabled: widget.connected

        anchors {
            bottom: parent.bottom
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
            top: titleField.bottom
            topMargin: 4
        }

        // TODO: Fix clipping
        RadialGauge {
            id: gauge

            endAngle: widget.endAngle
            maxValue: max
            minValue: min
            numTicks: ticks
            startAngle: widget.startAngle
            value: 0
            valueFontSize: fontSize

            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
