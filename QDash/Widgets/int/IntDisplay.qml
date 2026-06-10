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

    property color fontColor: Clover.theme.currentAccent
    property int maxFontSize: QDashSettings.defaultDisplayFontSize

    function update(value) {
        widget.connected = true
        txt.text = value
    }

    propertyKeys: ["maxFontSize", "fontColor"]

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
            bindedProperty: "color"
            label: "Text Color"
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
                text: "Dial"

                onTriggered: {
                    model.type = "dial"
                }
            }
        }
    }

    Text {
        id: txt

        color: widget.fontColor
        enabled: widget.connected
        font.pixelSize: maxFontSize
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
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
