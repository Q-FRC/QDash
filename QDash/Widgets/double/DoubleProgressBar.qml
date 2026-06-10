// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

// TODO: Make vertical automatically based on height/width
PrimitiveWidget {
    id: widget

    propertyKeys: ["fontSize", "numTicks", "suffix", "lowerBound", "upperBound", "vertical"]
    menuExtension: Component {
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

            MenuItem {
                text: "Phase Display"

                onTriggered: {
                    model.type = "phaseShift"
                }
            }
        }
    }

    property int fontSize: QDashSettings.defaultFontSize
    property double lowerBound: 0.0
    property int numTicks: 5
    property string suffix: ""
    property double upperBound: 100.0
    property bool vertical: false

    function update(value) {
        widget.connected = true
        bar.value = value
    }

    ProgressBar {
        id: bar

        enabled: widget.connected

        width: vertical ? (parent.height - titleField.height - 50) : (parent.width - 50)
        rotation: vertical ? -90 : 0

        font.pixelSize: fontSize

        from: lowerBound
        to: upperBound

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: titleField.height / 2
        }

        Repeater {
            model: numTicks + 1

            Item {
                x: (bar.width * index) / numTicks

                anchors {
                    top: bar.bottom
                }

                Rectangle {
                    id: tick

                    color: Clover.theme.text
                    height: 10
                    width: 2
                }

                Text {
                    color: Clover.theme.text
                    font.pixelSize: 15
                    rotation: vertical ? 90 : 0
                    text: (bar.from + index * (bar.to - bar.from) / numTicks).toFixed(1) + suffix

                    anchors {
                        horizontalCenter: tick.horizontalCenter
                        top: tick.bottom
                        topMargin: 15
                    }
                }
            }
        }

        Text {
            id: txt

            color: Clover.theme.text
            font.pixelSize: fontSize

            rotation: vertical ? 90 : 0
            text: parent.value + suffix

            anchors {
                bottom: bar.top
                bottomMargin: 25
                horizontalCenter: parent.horizontalCenter
            }
        }
    }

    configContent: ColumnLayout {
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
            label: "Bar Settings"
        }

        RowLayout {
            LabeledDoubleSpinBox {
                bindedProperty: "lowerBound"
                label: "Lower Bound"
            }

            LabeledDoubleSpinBox {
                bindedProperty: "upperBound"
                label: "Upper Bound"
            }
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "numTicks"
                label: "Number of Ticks"
                stepSize: 1
            }

            LabeledTextField {
                bindedProperty: "suffix"
                label: "Suffix"
            }
        }

        LabeledCheckbox {
            bindedProperty: "vertical"
            label: "Vertical?"
        }

        SectionHeader {
            label: "NT Settings"
        }

        // TODO: Automatically append this
        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
        }
    }
}
