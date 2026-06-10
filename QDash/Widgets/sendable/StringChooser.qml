// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Components
import QDash.Controls
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

SendableWidget {
    id: widget

    property int fontSize: 14
    property bool readyToUpdate: true

    function update(topic, value) {
        widget.connected = true
        switch (topic) {
        case "options":
            {
                combo.choices = value
                break
            }
        case "active":
            {
                if (!readyToUpdate) {
                    readyToUpdate = true
                    return
                }

                button.valid = true
                combo.currentIndex = combo.indexOfValue(value)
                combo.previousIndex = combo.currentIndex

                break
            }
        }
    }

    propertyKeys: ["fontSize"]
    topics: ["options", "active", "selected"]

    // TODO(crueter): Alongside deduping the loader stuff, most of this is just
    // type-label-property, with maybe a few extras... possible schema candidate?
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
            label: "NT Settings"
        }

        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
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

        SearchableComboBox {
            id: combo

            property int previousIndex: -1

            enabled: widget.connected
            font.pixelSize: fontSize
            implicitHeight: 40

            onActivated: index => {
                if (previousIndex !== index) {
                    button.valid = false
                }

                previousIndex = index

                widget.setValue("selected", valueAt(index))
            }

            anchors {
                left: parent.left
                right: button.left
                verticalCenter: parent.verticalCenter
            }

            Connections {
                function onConnected(conn) {
                    if (conn) {
                        if (combo.previousIndex !== -1) {
                            logs.info("StringChooser", "Force-updating chooser \"" + item_topic + "\" to value " + combo.currentText)

                            widget.readyToUpdate = false
                            widget.setValue("selected", combo.currentText)
                        }
                    }
                }

                target: TopicStore
            }
        }

        Button {
            id: button

            property bool valid: widget.valid

            background: Item {}

            icon {
                color: valid ? "green" : "red"
                source: valid ? "qrc:/Valid" : "qrc:/Invalid"
            }

            anchors {
                margins: 2
                right: parent.right
                verticalCenter: combo.verticalCenter
            }
        }
    }
}
