// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Components
import QDash.Controls
import QDash.Widgets

import Carboxyl.Clover

SendableWidget {
    id: widget

    topics: ["options", "active", "selected"]

    propertyKeys: ["fontSize"]

    property int fontSize: 14

    property bool readyToUpdate: true

    function update(topic, value) {
        widget.connected = true;
        switch (topic) {
        case "options":
            {
                combo.choices = value;
                break;
            }
        case "active":
            {
                if (!readyToUpdate) {
                    readyToUpdate = true;
                    return;
                }

                button.valid = true;
                combo.currentIndex = combo.indexOfValue(value);
                combo.previousIndex = combo.currentIndex;

                break;
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

        SearchableComboBox {
            id: combo

            enabled: widget.connected

            anchors {
                verticalCenter: parent.verticalCenter

                left: parent.left
                right: button.left
            }

            font.pixelSize: fontSize

            implicitHeight: 40

            property int previousIndex: -1

            Connections {
                target: TopicStore

                function onConnected(conn) {
                    if (conn) {
                        if (combo.previousIndex !== -1) {
                            logs.info("StringChooser", "Force-updating chooser \"" + item_topic + "\" to value " + combo.currentText);

                            widget.readyToUpdate = false;
                            widget.setValue("selected", combo.currentText);
                        }
                    }
                }
            }

            onActivated: index => {
                if (previousIndex !== index) {
                    button.valid = false;
                }

                previousIndex = index;

                widget.setValue("selected", valueAt(index));
            }
        }

        Button {
            id: button

            property bool valid: widget.valid

            icon {
                color: valid ? "green" : "red"
                source: valid ? "qrc:/Valid" : "qrc:/Invalid"
            }

            background: Item {}

            anchors {
                verticalCenter: combo.verticalCenter
                right: parent.right

                margins: 2
            }
        }
    }

    // TODO(crueter): Alongside deduping the loader stuff, most of this is just
    // type-label-property, with maybe a few extras... possible schema candidate?
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
            label: "NT Settings"
        }

        LabeledTextField {
            label: "Topic"
            bindedProperty: "item_topic"
        }
    }
}
