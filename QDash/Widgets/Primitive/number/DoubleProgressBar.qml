// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int fontSize: QDashSettings.defaultFontSize

    property int numTicks: 5

    property string suffix: ""

    property double lowerBound: 0.0
    property double upperBound: 100.0

    property bool vertical: false

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

    function update(value) {
        widget.connected = true
        bar.value = value
    }

    ProgressBar {
        id: bar

        font.pixelSize: fontSize

        from: lowerBound
        to: upperBound

        rotation: vertical ? -90 : 0
        enabled: widget.connected

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: titleField.height / 2
        }

        width: vertical ? (parent.height - titleField.height - 50) : (parent.width - 50)

        Repeater {
            model: numTicks + 1

            Item {
                anchors {
                    top: bar.bottom
                }

                x: (bar.width * index) / numTicks

                Rectangle {
                    id: tick
                    width: 2
                    height: 10

                    color: Clover.theme.text
                }

                Text {
                    color: Clover.theme.text

                    text: (bar.from + index * (bar.to - bar.from) / numTicks).toFixed(
                              1) + suffix

                    font.pixelSize: 15

                    anchors {
                        top: tick.bottom
                        topMargin: 15

                        horizontalCenter: tick.horizontalCenter
                    }

                    rotation: vertical ? 90 : 0
                }
            }
        }

        Text {
            id: txt

            color: Clover.theme.text
            font.pixelSize: fontSize

            text: parent.value + suffix
            rotation: vertical ? 90 : 0

            anchors {
                bottom: bar.top
                bottomMargin: 25

                horizontalCenter: parent.horizontalCenter
            }
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
                            Layout.fillWidth: true

                            id: titleFontField

                            label: "Title Font Size"

                            bindedProperty: "titleFontSize"
                            
                        }

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: fontField

                            label: "Font Size"

                            bindedProperty: "fontSize"
                            
                        }
                    }

                    SectionHeader {
                        label: "Bar Settings"
                    }

                    RowLayout {

                        LabeledDoubleSpinBox {
                            Layout.fillWidth: true

                            id: lowField

                            label: "Lower Bound"

                            bindedProperty: "lowerBound"
                            
                        }

                        LabeledDoubleSpinBox {
                            Layout.fillWidth: true

                            id: upField

                            label: "Upper Bound"

                            bindedProperty: "upperBound"
                            
                        }
                    }

                    RowLayout {

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: tickField

                            label: "Number of Ticks"

                            bindedProperty: "numTicks"
                            

                            stepSize: 1
                        }

                        LabeledTextField {
                            Layout.fillWidth: true

                            id: suffixField

                            label: "Suffix"

                            bindedProperty: "suffix"
                            
                        }
                    }

                    LabeledCheckbox {
                        Layout.fillWidth: true

                        id: vertField

                        label: "Vertical?"

                        bindedProperty: "vertical"
                        
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
                        
                    }
                }
            }
        }
    }
}
