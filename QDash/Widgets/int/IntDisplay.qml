// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Controls
import QDash.Widgets

import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int maxFontSize: QDashSettings.defaultDisplayFontSize
    property color fontColor: Clover.theme.currentAccent

    propertyKeys: ["maxFontSize", "fontColor"]

    menuExtension: Component {
        Menu {
            id: switchMenu
            title: "Switch Widget..."

            MenuItem {
                text: "Spin Box"
                onTriggered: {
                    model.type = "int";
                }
            }

            MenuItem {
                text: "Dial"
                onTriggered: {
                    model.type = "dial";
                }
            }
        }
    }

    function update(value) {
        widget.connected = true;
        txt.text = value;
    }

    Text {
        id: txt

        font.pixelSize: maxFontSize

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

    configContent: ColumnLayout {
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
                label: "Title Font Size"
                bindedProperty: "titleFontSize"
            }

            LabeledSpinBox {
                label: "Maximum Font Size"
                bindedProperty: "maxFontSize"
            }
        }

        SectionHeader {
            label: "Display Settings"
        }

        ColorField {
            label: "Text Color"
            bindedProperty: "color"
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            label: "Topic"
            bindedProperty: "item_topic"
        }
    }
}
