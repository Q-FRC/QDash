// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QDash.Controls

import QDash.Core
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

PrimitiveWidget {
    id: widget

    readOnly: false
    roleString: "double"
    typeString: "double"
    widgetLabel: "Spin Box"

    propertyKeys: ["stepSize", "fontSize", "lowerBound", "upperBound"]
    property int fontSize: QDashSettings.defaultFontSize
    property double lowerBound: -100000.0
    property double stepSize: 0.1
    property double upperBound: 100000.0

    function update(value) {
        widget.connected = true
        spin.value = value
    }

    Item {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: titleField.bottom

            leftMargin: 10
            rightMargin: 10
        }

        DoubleSpinBox {
            id: spin

            editable: true
            enabled: widget.connected

            font.pixelSize: fontSize

            from: lowerBound
            to: upperBound
            stepSize: stepSize

            value: 0

            onValueModified: widget.setValue(value)

            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
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
                bindedProperty: "fontSize"
                label: "Font Size"
            }
        }

        SectionHeader {
            label: "Spin Box Settings"
        }

        RowLayout {
            LabeledDoubleSpinBox {
                bindedProperty: "lowerBound"
                label: "Lower Bound"
            }

            LabeledDoubleSpinBox {
                bindedProperty: "upperBound"
                label: "Upper Bound"
            }
        }

        LabeledDoubleSpinBox {
            bindedProperty: "stepSize"
            from: 0
            label: "Step Size"
            stepSize: 0.1
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
