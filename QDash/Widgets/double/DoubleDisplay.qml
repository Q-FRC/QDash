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

    readOnly: true
    roleString: "doubleDisplay"
    typeString: "double"
    widgetLabel: "Number Display"

    propertyKeys: ["maxFontSize", "decimals", "fontColor"]
    property int decimals: 2
    property color fontColor: Clover.theme.currentAccent
    property int maxFontSize: QDashSettings.defaultDisplayFontSize

    function update(value) {
        widget.connected = true
        txt.value = value
    }

    Text {
        id: txt

        property double value

        font.pixelSize: maxFontSize
        fontSizeMode: Text.Fit
        enabled: widget.connected

        color: widget.fontColor
        text: value.toFixed(decimals)

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: titleField.bottom

            margins: 10
        }
    }

    configComponent: Component {
        ColumnLayout {
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
    }
}
