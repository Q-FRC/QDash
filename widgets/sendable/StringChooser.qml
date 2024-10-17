import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QFRCDashboard

BaseWidget {
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
                font.pixelSize: item_fontSize
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
            highlighted: combo.highlightedIndex === index
        }

        font.pixelSize: item_fontSize

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
}
