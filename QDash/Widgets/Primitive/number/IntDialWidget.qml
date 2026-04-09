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

    property int fontSize: 20

    property double startAngle: 180
    property double endAngle: 540

    property int stepSize: 1

    property int lowerBound: -1000
    property int upperBound: 1000

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
    }

    function update(value) {
        widget.connected = true
        spin.value = value
        dial.value = value
    }

    SpinBox {
        id: spin

        font.pixelSize: widget.fontSize

        value: 0

        from: widget.lowerBound
        to: widget.upperBound
        stepSize: widget.stepSize

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

        font.pixelSize: widget.fontSize

        value: 0
        stepSize: widget.stepSize

        width: Math.min(parent.width, spin.y - titleField.height - 40)
        height: width

        from: widget.lowerBound
        to: widget.upperBound

        startAngle: widget.startAngle
        endAngle: widget.endAngle

        enabled: widget.connected

        anchors {
            top: titleField.bottom
            horizontalCenter: parent.horizontalCenter

            margins: 20
        }

        onMoved: spin.move(parseInt(value))
    }

    // TODO: Remove these IDs. We don't need them anymore (besides config)
    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: Component {
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
                        label: "Spin Box Settings"
                    }

                    RowLayout {

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: lowField

                            label: "Lower Bound"

                            bindedProperty: "lowerBound"
                            bindTarget: widget
                        }

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: upField

                            label: "Upper Bound"

                            bindedProperty: "upperBound"
                            bindTarget: widget
                        }
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: stepField

                        label: "Step Size"

                        bindedProperty: "stepSize"
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
}
