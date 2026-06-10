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

    property int fontSize: QDashSettings.defaultFontSize

    property int stepSize: 1

    property int upperBound: 100000
    property int lowerBound: -100000

    propertyKeys: ["fontSize", "stepSize", "upperBound", "lowerBound"]

    menuExtension: Component {
        Menu {
            id: switchMenu
            title: "Switch Widget..."

            MenuItem {
                text: "Dial"
                onTriggered: {
                    model.type = "dial";
                }
            }

            MenuItem {
                text: "Number Display"
                onTriggered: {
                    model.type = "intDisplay";
                }
            }
        }
    }

    function update(value) {
        widget.connected = true;
        spin.value = value;
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        SpinBox {
            id: spin

            font.pixelSize: fontSize

            value: 0
            from: lowerBound
            to: upperBound
            stepSize: stepSize

            enabled: widget.connected
            editable: true

            // TODO: Bring back validity handling
            // valid: widget.valid
            anchors {
                verticalCenter: parent.verticalCenter

                left: parent.left
                right: parent.right
            }

            onValueModified: {
                widget.setValue(value);
            }
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
                label: "Font Size"
                bindedProperty: "fontSize"
            }
        }

        SectionHeader {
            label: "Spin Box Settings"
        }

        RowLayout {
            LabeledSpinBox {
                label: "Lower Bound"
                bindedProperty: "lowerBound"
            }

            LabeledSpinBox {
                label: "Upper Bound"
                bindedProperty: "upperBound"
            }
        }

        LabeledSpinBox {
            label: "Step Size"
            bindedProperty: "stepSize"
            from: 0
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
