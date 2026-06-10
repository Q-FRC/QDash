// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

SendableWidget {
    id: widget

    readOnly: true
    roleString: "Command"

    propertyKeys: ["fontSize"]
    topics: [".name", "running"]

    property int fontSize: 18

    function update(topic, value) {
        widget.connected = true

        switch (topic) {
        case ".name":
            {
                cmdButton.name = value
                break
            }
        case "running":
            {
                cmdButton.running = value
                break
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

        Button {
            id: cmdButton

            property string name: "Command"
            property bool running: false

            enabled: widget.connected
            font.pixelSize: fontSize
            text: name

            onClicked: {
                running = !running
                widget.setValue("running", running)
            }

            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
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
                    bindedProperty: "fontSize"
                    label: "Font Size"
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
