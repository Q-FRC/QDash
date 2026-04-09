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

    property int fontSize: 20

    property int stepSize: 1

    property int upperBound: 100000
    property int lowerBound: -100000

    propertyKeys: ["fontSize", "stepSize", "upperBound", "lowerBound"]

    dataType: "int"
    widgetType: "int"

    function update(value) {
        widget.connected = true
        spin.value = value
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
                widget.setValue(value)
            }
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

                            label: "Font Size"

                            bindedProperty: "fontSize"
                            bindTarget: widget
                        }
                    }

                    SectionHeader {
                        label: "Spin Box Settings"
                    }

                    RowLayout {

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: lowField

                            label: "Lower Bound"

                            bindedProperty: "lowerBound"
                            bindTarget: widget
                        }

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: upField

                            label: "Upper Bound"

                            bindedProperty: "upperBound"
                            bindTarget: widget
                        }
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        id: stepField

                        label: "Step Size"

                        bindedProperty: "stepSize"
                        bindTarget: widget

                        from: 0
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
