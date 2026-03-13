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
    property color fontColor: Clover.theme.currentAccent
    property bool wrap: true

    propertyKeys: ["fontSize", "fontColor", "wrap"]

    menuExtension: Component {
        Menu {
            id: switchMenu
            title: "Switch Widget..."

            MenuItem {
                text: "Text Field"
                onTriggered: {
                    model.type = "string"
                }
            }

            MenuItem {
                text: "Color"
                onTriggered: {
                    model.type = "colorText"
                }
            }
        }
    }

    function update(value) {
        widget.connected = true
        txt.text = value
    }

    Text {
        id: txt

        font.pixelSize: fontSize

        property string value

        color: widget.fontColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        fontSizeMode: Text.Fit

        wrapMode: wrap ? Text.WrapAtWordBoundaryOrAnywhere : Text.NoWrap
        enabled: widget.connected

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

        sourceComponent: BaseConfigDialog {
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
                RowLayout {

                    ColorField {
                        Layout.fillWidth: true

                        id: colorField

                        label: "Text Color"

                        bindedProperty: "color"
                        bindTarget: widget
                    }

                    LabeledCheckbox {
                        Layout.fillWidth: true

                        id: wrapField

                        label: "Wrap Text?"

                        bindedProperty: "wrap"
                        bindTarget: widget
                    }
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
