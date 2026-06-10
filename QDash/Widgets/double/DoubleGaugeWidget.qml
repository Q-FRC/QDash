// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Controls
import QDash.Core
import QDash.Widgets

import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int fontSize: QDashSettings.defaultFontSize

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
                    model.type = "doubleDial";
                }
            }

            MenuItem {
                text: "Spin Box"
                onTriggered: {
                    model.type = "double";
                }
            }

            MenuItem {
                text: "Progress Bar"
                onTriggered: {
                    model.type = "doubleBar";
                }
            }

            MenuItem {
                text: "Number Display"
                onTriggered: {
                    model.type = "doubleDisplay";
                }
            }

            MenuItem {
                text: "Match Time"
                onTriggered: {
                    model.type = "matchTime";
                }
            }

            MenuItem {
                text: "Phase Display"
                onTriggered: {
                    model.type = "phaseShift";
                }
            }
        }
    }

    function fixGaugeSize() {
        gauge.width = width;
        gauge.height = height - titleField.height;
        gauge.fixGaugeSize();
    }

    Component.onCompleted: fixGaugeSize()
    onHeightChanged: fixGaugeSize()
    onWidthChanged: fixGaugeSize()

    function update(value) {
        widget.connected = true;
        gauge.value = value;
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

            startAngle: widget.startAngle
            endAngle: widget.endAngle
        }
    }

    configContent: ColumnLayout {
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
                label: "Title Font Size"
                bindedProperty: "titleFontSize"
            }

            LabeledSpinBox {
                label: "Font Size"
                bindedProperty: "fontSize"
            }
        }

        SectionHeader {
            label: "Gauge Settings"
        }

        LabeledSpinBox {
            label: "Number of Ticks"
            bindedProperty: "ticks"
        }

        RowLayout {
            LabeledDoubleSpinBox {
                label: "Minimum Value"
                bindedProperty: "min"
            }

            LabeledDoubleSpinBox {
                label: "Maximum Value"
                bindedProperty: "max"
            }
        }

        RowLayout {
            LabeledDoubleSpinBox {
                label: "Start Angle"
                bindedProperty: "startAngle"
            }

            LabeledDoubleSpinBox {
                label: "End Angle"
                bindedProperty: "endAngle"
            }
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            label: "Topic"
            bindedProperty: "item_topic"
        }
    }
}
