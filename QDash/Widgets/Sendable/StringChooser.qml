import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import QDash.Constants

SendableWidget {
    id: widget

    topics: ["options", "active", "selected"]

    property int item_fontSize: 14

    property bool readyToUpdate: true

    function update(topic, value) {
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

            valid: widget.valid
            connected: widget.connected

            anchors {
                verticalCenter: parent.verticalCenter

                left: parent.left
                right: button.left
            }

            font.pixelSize: item_fontSize

            implicitHeight: 40

            property int previousIndex: -1

            Connections {
                target: topicStore

                function onConnected(conn) {
                    if (conn) {
                        if (combo.previousIndex !== -1) {
                            logs.info("StringChooser",
                                      "Force-updating chooser \"" + item_topic
                                      + "\" to value " + combo.currentText)

                            widget.readyToUpdate = false
                            widget.setValue("selected", combo.currentText)
                        }
                    }
                }
            }

            onActivated: index => {
                             if (previousIndex !== index) {
                                 button.valid = false
                             }

                             previousIndex = index

                             widget.setValue("selected", valueAt(index))
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
                uniformCellSizes: true

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: titleFontField

                    label: "Title Font Size"

                    bindedProperty: "item_titleFontSize"
                    bindTarget: widget
                }

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: fontField

                    label: "Font Size"

                    bindedProperty: "item_fontSize"
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
