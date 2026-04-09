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

    property double stepSize: 0.1

    property int fontSize: 20

    property double startAngle: -180
    property double endAngle: 180

    property double lowerBound: -100000.0
    property double upperBound: 100000.0

    propertyKeys: ["stepSize", "fontSize", "startAngle", "endAngle", "lowerBound", "upperBound"]

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
    }

    function update(value) {
        widget.connected = true
        spin.value = value
        dial.value = value
    }

    DoubleSpinBox {
        id: spin

        font.pixelSize: widget.fontSize

        enabled: widget.connected
        editable: true

        value: 0

        from: widget.lowerBound
        to: widget.upperBound
        stepSize: widget.stepSize

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

        onMoved: {
            spin.move(value)
        }
    }

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
                            // TODO: commonize fillWidth
                            Layout.fillWidth: true

                            label: "Title Font Size"
                            bindedProperty: "titleFontSize"
                        }

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            label: "Font Size"
                            bindedProperty: "fontSize"
                        }
                    }

                    SectionHeader {
                        label: "Spin Box Settings"
                    }

                    RowLayout {

                        LabeledDoubleSpinBox {
                            Layout.fillWidth: true

                            label: "Lower Bound"
                            bindedProperty: "lowerBound"
                        }

                        LabeledDoubleSpinBox {
                            Layout.fillWidth: true

                            label: "Upper Bound"
                            bindedProperty: "upperBound"
                        }
                    }

                    LabeledDoubleSpinBox {
                        Layout.fillWidth: true

                        label: "Step Size"
                        bindedProperty: "stepSize"

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
                            bindedProperty: "startAngle"
                        }

                        LabeledDoubleSpinBox {
                            Layout.fillWidth: true

                            label: "End Angle"
                            bindedProperty: "endAngle"
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
    }
}
