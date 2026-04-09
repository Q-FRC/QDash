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

    property int fontSize: 100
    property color warningColor: "yellow"

    propertyKeys: ["fontSize", "warningColor"]

    dataType: "double"
    widgetType: "matchTime"

    function update(value) {
        txt.value = Math.ceil(value)
    }

    Text {
        id: txt

        font.pixelSize: fontSize

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
                            Layout.fillWidth: true

                            id: titleFontField

                            label: "Title Font Size"

                            bindedProperty: "titleFontSize"
                            bindTarget: widget
                        }

                        LabeledSpinBox {
                            Layout.fillWidth: true

                            id: fontField

                            label: "Maximum Font Size"

                            bindedProperty: "fontSize"
                            bindTarget: widget
                        }
                    }

                    SectionHeader {
                        label: "Display Settings"
                    }

                    ColorField {
                        Layout.fillWidth: true

                        id: warnField

                        label: "Warning Color"

                        bindedProperty: "warningColor"
                        bindTarget: widget
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
