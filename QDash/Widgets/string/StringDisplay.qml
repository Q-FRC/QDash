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
    roleString: "textDisplay"
    typeString: "string"
    widgetLabel: "Text Display"

    propertyKeys: ["maxFontSize", "fontColor", "wrap"]
    property color fontColor: Clover.theme.currentAccent
    property int maxFontSize: QDashSettings.defaultDisplayFontSize
    property bool wrap: true

    function update(value) {
        widget.connected = true
        txt.text = value
    }

    Text {
        id: txt

        property string value

        color: widget.fontColor
        enabled: widget.connected
        font.pixelSize: maxFontSize
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: wrap ? Text.WrapAtWordBoundaryOrAnywhere : Text.NoWrap

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 10
            right: parent.right
            top: titleField.bottom
        }
    }

    configContent: ColumnLayout {
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
            ColorField {
                bindedProperty: "color"
                label: "Text Color"
            }

            LabeledCheckbox {
                bindedProperty: "wrap"
                label: "Wrap Text?"
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
