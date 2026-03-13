// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int maxFontSize: 100
    property int decimals: 2
    property color fontColor: Clover.theme.currentAccent

    propertyKeys: ["maxFontSize", "decimals", "fontColor"]

    // TODO(crueter): There should be some kind of interface that maps names to model types
    // That way we can just use Repeater or whatever, or define all the widget types for a specific
    // type???
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
                text: "Progress Bar"
                onTriggered: {
                    model.type = "doubleBar"
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
        txt.value = value
    }

    Text {
        id: txt

        font.pixelSize: maxFontSize

        property double value

        text: value.toFixed(decimals)

        color: widget.fontColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        fontSizeMode: Text.Fit
        enabled: widget.connected

        anchors {
            top: titleField.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom

            margins: 10
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
                            bindTarget: widget
                        }

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: fontField

                            label: "Maximum Font Size"

                            bindedProperty: "maxFontSize"
                            bindTarget: widget
                        }
                    }

                    SectionHeader {
                        label: "Display Settings"
                    }

                    RowLayout {

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: decField

                            label: "Number of Decimals"

                            bindedProperty: "decimals"
                            bindTarget: widget

                            from: 0
                        }

                        ColorField {
                            Layout.fillWidth: true

                            id: colorField

                            label: "Text Color"

                            bindedProperty: "color"
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
