// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls 6.4
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property double item_stepSize: 0.1

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

        MenuItem {
            text: "Phase Display"
            onTriggered: {
                model.type = "phaseShift"
            }
        }
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    function update(value) {
        widget.connected = true
        spin.value = value
        dial.value = value
    }

    DoubleSpinBox {
        id: spin

        font.pixelSize: item_fontSize

        enabled: widget.connected
        editable: true

        value: 0

        from: item_lowerBound
        to: item_upperBound
        stepSize: item_stepSize

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            margins: 10
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

        inputMode: Dial.Circular

        font.pixelSize: item_fontSize

        value: 0
        stepSize: item_stepSize

        width: Math.min(parent.width, spin.y - titleField.height - 40)
        height: width

        from: item_lowerBound
        to: item_upperBound

        startAngle: item_startAngle
        endAngle: item_endAngle

        enabled: widget.connected

        anchors {
            top: titleField.bottom
            horizontalCenter: parent.horizontalCenter

            margins: 20
        }

        onMoved: {
            spin.move(value)
        }
    }

    BaseConfigDialog {
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
                    // TODO: commonize fillWidth
                    Layout.fillWidth: true

                    label: "Title Font Size"
                    bindedProperty: "item_titleFontSize"
                }

                LabeledSpinBox {
                    Layout.fillWidth: true

                    label: "Font Size"
                    bindedProperty: "item_fontSize"
                }
            }

            SectionHeader {
                label: "Spin Box Settings"
            }

            RowLayout {

                LabeledDoubleSpinBox {
                    Layout.fillWidth: true

                    label: "Lower Bound"
                    bindedProperty: "item_lowerBound"
                }

                LabeledDoubleSpinBox {
                    Layout.fillWidth: true

                    label: "Upper Bound"
                    bindedProperty: "item_upperBound"
                }
            }

            LabeledDoubleSpinBox {
                Layout.fillWidth: true

                label: "Step Size"
                bindedProperty: "item_stepSize"

                from: 0
                stepSize: 0.1
            }

            SectionHeader {
                label: "Dial Settings"
            }

            RowLayout {

                LabeledDoubleSpinBox {
                    Layout.fillWidth: true

                    label: "Start Angle"
                    bindedProperty: "item_startAngle"
                }

                LabeledDoubleSpinBox {
                    Layout.fillWidth: true

                    label: "End Angle"
                    bindedProperty: "item_endAngle"
                }
            }

            SectionHeader {
                label: "NT Settings"
            }

            LabeledTextField {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                label: "Topic"
                bindedProperty: "item_topic"
            }
        }
    }
}
