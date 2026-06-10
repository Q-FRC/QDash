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

    property int fontSize: QDashSettings.defaultFontSize
    property int lowerBound: -100000
    property int stepSize: 1
    property int upperBound: 100000

    function update(value) {
        widget.connected = true
        spin.value = value
    }

    propertyKeys: ["fontSize", "stepSize", "upperBound", "lowerBound"]

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
                bindedProperty: "fontSize"
                label: "Font Size"
            }
        }

        SectionHeader {
            label: "Spin Box Settings"
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "lowerBound"
                label: "Lower Bound"
            }

            LabeledSpinBox {
                bindedProperty: "upperBound"
                label: "Upper Bound"
            }
        }

        LabeledSpinBox {
            bindedProperty: "stepSize"
            from: 0
            label: "Step Size"
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
                text: "Dial"

                onTriggered: {
                    model.type = "dial"
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

    Item {
        anchors {
            bottom: parent.bottom
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
            top: titleField.bottom
        }

        SpinBox {
            id: spin

            editable: true
            enabled: widget.connected
            font.pixelSize: fontSize
            from: lowerBound
            stepSize: stepSize
            to: upperBound
            value: 0

            onValueModified: {
                widget.setValue(value)
            }

            // TODO: Bring back validity handling
            // valid: widget.valid
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }
    }
}
