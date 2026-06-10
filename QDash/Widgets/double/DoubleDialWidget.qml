// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QDash.Controls

import QDash.Core
import QDash.Widgets
import QtQuick
import QtQuick.Controls 6.8
import QtQuick.Layouts

PrimitiveWidget {
    id: widget

    readOnly: false
    roleString: "doubleDial"
    typeString: "double"
    widgetLabel: "Dial"

    propertyKeys: ["stepSize", "fontSize", "startAngle", "endAngle", "lowerBound", "upperBound"]
    property double endAngle: 180
    property int fontSize: QDashSettings.defaultFontSize
    property double lowerBound: -100000.0
    property double startAngle: -180
    property double stepSize: 0.1
    property double upperBound: 100000.0

    function update(value) {
        widget.connected = true
        spin.value = value
        dial.value = value
    }

    DoubleSpinBox {
        id: spin

        function move(val) {
            let previousValue = value

            value = val
            widget.setValue(value)

            widget.valid = Math.abs(previousValue - value) < 0.01
        }

        editable: true
        enabled: widget.connected

        font.pixelSize: widget.fontSize

        from: widget.lowerBound
        to: widget.upperBound
        stepSize: widget.stepSize

        value: 0

        onValueModified: {
            dial.value = value
            widget.setValue(value)
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            margins: 10
        }
    }

    Dial {
        id: dial

        width: Math.min(parent.width, spin.y - titleField.height - 40)
        height: width

        enabled: widget.connected

        endAngle: widget.endAngle
        startAngle: widget.startAngle

        from: widget.lowerBound
        to: widget.upperBound
        stepSize: widget.stepSize

        inputMode: Dial.Circular
        value: 0

        onMoved: {
            spin.move(value)
        }

        anchors {
            horizontalCenter: parent.horizontalCenter
            margins: 20
            top: titleField.bottom
        }
    }

    configComponent: Component {
        ColumnLayout {
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
                    Layout.fillWidth: true
                    bindedProperty: "fontSize"
                    label: "Font Size"
                }
            }

            SectionHeader {
                label: "Spin Box Settings"
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

            LabeledDoubleSpinBox {
                bindedProperty: "stepSize"
                from: 0
                label: "Step Size"
                stepSize: 0.1
            }

            SectionHeader {
                label: "Dial Settings"
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
    }
}
