import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

SendableWidget {
    id: widget

    topics: [".name", "running"]

    property int item_fontSize: 18

    function update(topic, value) {
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

    Button {
        id: cmdButton

        anchors {
            verticalCenter: parent.verticalCenter

            left: parent.left
            right: parent.right
        }

        font.pixelSize: item_fontSize * Constants.scalar

        property bool running: false
        property string name: "Command"

        onClicked: {
            running = !running
            widget.setValue("running", running)
        }

        text: name
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
