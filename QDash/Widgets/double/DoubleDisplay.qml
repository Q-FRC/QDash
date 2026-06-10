// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

PrimitiveWidget {
    id: widget

    property int decimals: 2
    property color fontColor: Clover.theme.currentAccent
    property int maxFontSize: QDashSettings.defaultDisplayFontSize

    function update(value) {
        widget.connected = true
        txt.value = value
    }

    propertyKeys: ["maxFontSize", "decimals", "fontColor"]

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
                bindedProperty: "maxFontSize"
                label: "Maximum Font Size"
            }
        }

        SectionHeader {
            label: "Display Settings"
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "decimals"
                from: 0
                label: "Number of Decimals"
            }

            ColorField {
                bindedProperty: "color"
                label: "Text Color"
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

    Text {
        id: txt

        property double value

        color: widget.fontColor
        enabled: widget.connected
        font.pixelSize: maxFontSize
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        text: value.toFixed(decimals)
        verticalAlignment: Text.AlignVCenter

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 10
            right: parent.right
            top: titleField.bottom
        }
    }
}
