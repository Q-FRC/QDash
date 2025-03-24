import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property int item_fontSize: 14

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

        property string active

        property bool readyToUpdate: true

        property int previousIndex: 0

        function updateTopic(ntTopic, value) {
            if (ntTopic === item_topic + "/options") {
                choices = value
            } else if (ntTopic === item_topic + "/active") {
                if (!readyToUpdate) {
                    readyToUpdate = true
                    return
                }

                button.valid = true
                active = value
                currentIndex = indexOfValue(active)

                previousIndex = currentIndex
            }
        }

        // TODO: rewrite other widgets to use this
        Connections {
            target: topicStore

            function onConnected(conn) {
                if (conn) {
                    combo.readyToUpdate = false

                    button.valid = true
                    topicStore.setValue(item_topic + "/selected",
                                        combo.currentText)

                    combo.enabled = true
                } else {
                    button.valid = false
                    combo.enabled = false
                }
            }
        }

        function update() {
            topicStore.subscribe(item_topic + "/options")
            topicStore.subscribe(item_topic + "/active")
            topicStore.subscribe(item_topic + "/selected")
        }

        function unsubscribe() {
            topicStore.unsubscribe(topic + "/options")
            topicStore.unsubscribe(topic + "/active")
            topicStore.unsubscribe(topic + "/selected")
        }

        Component.onCompleted: {
            topicStore.topicUpdate.connect(updateTopic)
            item_topic = topic

            update()
        }

        Component.onDestruction: {
            if (topicStore !== null) {
                topicStore.topicUpdate.disconnect(updateTopic)
            }
        }

        onActivated: index => {
                         if (previousIndex !== index) {
                             button.valid = false
                         }

                         previousIndex = index

                         topicStore.setValue(item_topic + "/selected",
                                             valueAt(index))
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

    onItem_topicChanged: {
        combo.unsubscribe()
        topic = item_topic
        combo.update()
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
