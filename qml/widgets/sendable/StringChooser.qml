import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

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

    SearchableComboBox {
        id: combo

        anchors {
            verticalCenter: parent.verticalCenter

            left: parent.left
            right: button.left

            margins: 2
        }

        font.pixelSize: item_fontSize * Constants.scalar

        implicitHeight: 40 * Constants.scalar

        property int previousIndex: 0

        // TODO: rewrite other widgets to use this
        Connections {
            target: topicStore

            function onConnected(conn) {
                if (conn) {
                    widget.readyToUpdate = false

                    button.valid = true
                    widget.setValue("selected", combo.currentText)

                    combo.enabled = true
                } else {
                    button.valid = false
                    combo.enabled = false
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

        property bool valid: false

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

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fontField.accept()
        }

        ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: 5 * Constants.scalar

                rightMargin: 5
            }

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
