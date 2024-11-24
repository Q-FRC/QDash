import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property int item_fontSize: 18

    Button {
        id: cmdButton

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        font.pixelSize: item_fontSize

        property bool running: false
        property string name: "Command"

        function updateTopic(ntTopic, value) {
            if (ntTopic === item_topic + "/.name") {
                name = value
            } else if (ntTopic === item_topic + "/running") {
                running = value
            }
        }

        onClicked: {
            running = !running
            topicStore.setValue(item_topic + "/running", running)
        }

        text: name

        function update() {
            topicStore.subscribe(item_topic + "/.name")
            topicStore.subscribe(item_topic + "/running")
        }

        function unsubscribe() {
            topicStore.unsubscribe(item_topic + "/.name")
            topicStore.unsubscribe(item_topic + "/running")
        }

        Component.onCompleted: {
            topicStore.topicUpdate.connect(updateTopic)
            item_topic = model.topic
            update()
        }

        Component.onDestruction: {
            if (topicStore !== null) {
                topicStore.topicUpdate.disconnect(updateTopic)
                unsubscribe()
            }
        }
    }

    onItem_topicChanged: {
        cmdButton.unsubscribe()
        model.topic = item_topic

        cmdButton.update()
    }

    BaseConfigDialog {
        id: config

        height: 300

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
            spacing: 25

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: config.headerHeight + 12
                bottomMargin: 45

                leftMargin: 5
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
