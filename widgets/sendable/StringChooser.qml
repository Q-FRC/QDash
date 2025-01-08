import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property int item_fontSize: 18

    ComboBox {
        id: combo
        anchors {
            verticalCenter: parent.verticalCenter

            left: parent.left
            right: parent.right

            margins: 8
        }

        delegate: ItemDelegate {
            id: delegate

            width: combo.width
            contentItem: Text {
                text: modelData
                color: "white"
                font.pixelSize: item_fontSize * Constants.scalar
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
            highlighted: combo.highlightedIndex === index
        }

        font.pixelSize: item_fontSize * Constants.scalar

        implicitHeight: 40 * Constants.scalar

        property list<string> choices
        property string active

        property bool readyToUpdate: true

        model: choices

        function updateTopic(ntTopic, value) {
            if (ntTopic === item_topic + "/options") {
                choices = value
            } else if (ntTopic === item_topic + "/active") {
                if (!readyToUpdate) {
                    readyToUpdate = true;
                    return;
                }

                active = value
                currentIndex = indexOfValue(active)
            }
        }

        function connected(conn) {
            if (conn) {
                readyToUpdate = false;

                topicStore.setValue(item_topic + "/selected", currentText)
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

        onCurrentIndexChanged: {
            topicStore.setValue(item_topic + "/selected", valueAt(currentIndex))
        }
    }

    onItem_topicChanged: {
        combo.unsubscribe()
        topic = item_topic
        combo.update()
    }

    BaseConfigDialog {
        id: config

        height: 325 * Constants.scalar

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
            spacing: 25 * Constants.scalar

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: -20
                
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
