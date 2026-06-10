// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Controls
import QDash.Widgets

import Carboxyl.Clover

SendableWidget {
    id: widget

    topics: [".name", "running"]

    propertyKeys: ["fontSize"]

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
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        Button {
            id: cmdButton

            anchors {
                verticalCenter: parent.verticalCenter

                left: parent.left
                right: parent.right
            }

            font.pixelSize: fontSize
            enabled: widget.connected

            property bool running: false
            property string name: "Command"

            onClicked: {
                running = !running
                widget.setValue("running", running)
            }

            text: name
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
                            label: "Font Size"
                            bindedProperty: "fontSize"
                        }
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
