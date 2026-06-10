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
    property color warningColor: "yellow"

    propertyKeys: ["maxFontSize", "warningColor"]

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

    function update(value) {
        txt.value = Math.ceil(value)
    }

    Text {
        id: txt

        font.pixelSize: maxFontSize

        property double value

        text: Math.floor(value / 60) + ":" + String((value % 60).toFixed(
                                                        0)).padStart(2, '0')

        color: value < 30 ? warningColor : Clover.theme.currentAccent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        fontSizeMode: Text.Fit

        anchors {
            top: titleField.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom

            margins: 10
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
                        label: "Warning Color"
                        bindedProperty: "warningColor"
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
        }
    }
}
