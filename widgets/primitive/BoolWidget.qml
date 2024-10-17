import QtQuick 2.15
import QtQuick.Controls 2.15

import QFRCDashboard

BaseWidget {
    property string item_topic

    property int item_checkboxSize: 20

    property list<var> item_test: []

    property string testValueType: "color"
    property string testValueName: "Color"

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Color Display"
            onTriggered: {
                model.type = "color"
            }
        }
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    CheckBox {
        id: control

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                checked = ntValue
            }
        }

        checked: false

        anchors {
            centerIn: parent
        }

        indicator.implicitHeight: item_checkboxSize
        indicator.implicitWidth: item_checkboxSize

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

        onToggled: topicStore.setValue(item_topic, checked)
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic

        control.checked = topicStore.getValue(item_topic)
    }
}
