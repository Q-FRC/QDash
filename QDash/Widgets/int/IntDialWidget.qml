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

    propertyKeys: ["stepSize", "fontSize", "startAngle", "endAngle", "lowerBound", "upperBound"]

    // TODO: Remove these IDs. We don't need them anymore (besides config)
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

    SpinBox {
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
        stepSize: widget.stepSize
        to: widget.upperBound
        value: 0

        onValueModified: {
            dial.value = value
            widget.setValue(value)
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 10
            right: parent.right
        }
    }

    Dial {
        id: dial

        enabled: widget.connected
        endAngle: widget.endAngle
        font.pixelSize: widget.fontSize
        from: widget.lowerBound
        height: width
        inputMode: Dial.Circular
        startAngle: widget.startAngle
        stepSize: widget.stepSize
        to: widget.upperBound
        value: 0
        width: Math.min(parent.width, spin.y - titleField.height - 40)

        onMoved: spin.move(parseInt(value))

        anchors {
            horizontalCenter: parent.horizontalCenter
            margins: 20
            top: titleField.bottom
        }
    }
}
