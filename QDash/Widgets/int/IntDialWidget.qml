// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick
import QtQuick.Controls 6.8
import QtQuick.Layouts 6.8

PrimitiveWidget {
    id: widget

    propertyKeys: ["stepSize", "fontSize", "startAngle", "endAngle", "lowerBound", "upperBound"]
    menuExtension: Component {
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
                text: "Number Display"

                onTriggered: {
                    model.type = "intDisplay"
                }
            }
        }
    }

    property double endAngle: 540
    property int fontSize: QDashSettings.defaultFontSize
    property int lowerBound: -1000
    property double startAngle: 180
    property int stepSize: 1
    property int upperBound: 1000

    function update(value) {
        widget.connected = true
        spin.value = value
        dial.value = value
    }

    SpinBox {
        id: spin

        function move(val) {
            let previousValue = value

            value = val
            widget.setValue(value)

            widget.valid = Math.abs(previousValue - value) < 0.01
        }

        font.pixelSize: widget.fontSize

        editable: true
        enabled: widget.connected

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
        font.pixelSize: widget.fontSize

        endAngle: widget.endAngle
        startAngle: widget.startAngle

        from: widget.lowerBound
        to: widget.upperBound
        stepSize: widget.stepSize

        inputMode: Dial.Circular
        value: 0

        onMoved: spin.move(parseInt(value))

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: titleField.bottom

            margins: 20
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
            label: "Spin Box Settings"
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "lowerBound"
                label: "Lower Bound"
            }

            LabeledSpinBox {
                bindedProperty: "upperBound"
                label: "Upper Bound"
            }
        }

        LabeledSpinBox {
            bindedProperty: "stepSize"
            from: 0
            label: "Step Size"
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
