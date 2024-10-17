import QtQuick 2.15
import QtQuick.Controls 2.15

import QFRCDashboard

BaseWidget {
    property string item_topic

    property int item_fontSize: 15

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Enum"
            onTriggered: {
                model.type = "enum"
            }
        }
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    TextField {
        id: textField

        font.pixelSize: item_fontSize

        function updateTopic(ntTopic, value) {
            if (ntTopic === item_topic) {
                text = value
            }
        }

        text: ""

        anchors {
            verticalCenter: parent.verticalCenter
            topMargin: titleField.height

            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        Component.onCompleted: {
            topicStore.topicUpdate.connect(updateTopic)
            item_topic = model.topic
        }

        Component.onDestruction: {
            if (topicStore !== null) {
                topicStore.topicUpdate.disconnect(updateTopic)
                topicStore.unsubscribe(item_topic)
            }
        }

        onTextEdited: topicStore.setValue(item_topic, text)
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic
        textField.text = topicStore.getValue(item_topic)
    }
}
