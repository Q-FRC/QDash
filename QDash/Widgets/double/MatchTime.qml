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

    property int maxFontSize: QDashSettings.defaultDisplayFontSize
    property color warningColor: "yellow"

    function update(value) {
        txt.value = Math.ceil(value)
    }

    propertyKeys: ["maxFontSize", "warningColor"]

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

        ColorField {
            bindedProperty: "warningColor"
            label: "Warning Color"
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
                text: "Number Display"

                onTriggered: {
                    model.type = "doubleDisplay"
                }
            }
        }
    }

    Text {
        id: txt

        property double value

        color: value < 30 ? warningColor : Clover.theme.currentAccent
        font.pixelSize: maxFontSize
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        text: Math.floor(value / 60) + ":" + String((value % 60).toFixed(0)).padStart(2, '0')
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
