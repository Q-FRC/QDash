// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls 6.4
import QtQuick.Layouts 6.4

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int item_fontSize: 20

    property double item_startAngle: 180
    property double item_endAngle: 540

    property int item_stepSize: 1

    property int item_lowerBound: -1000
    property int item_upperBound: 1000

    Menu {
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
        widget.connected = true
        spin.value = value
        dial.value = value
    }

    SpinBox {
        id: spin

        font.pixelSize: item_fontSize

        value: 0

        from: item_lowerBound
        to: item_upperBound
        stepSize: item_stepSize

        enabled: widget.connected
        editable: true

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

        onMoved: spin.move(parseInt(value))
    }

    // TODO: Remove these IDs. We don't need them anymore (besides config)
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
