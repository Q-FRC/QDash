import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property list<string> topics
    property bool valid: true

    // Define this in your widget
    // this takes in the suffix only
    function update(topic, value) {}

    function setValue(topic, value) {
        valid = false
        topicStore.setValue(item_topic + "/" + topic, value)
    }

    function updateTopic(ntTopic, ntValue) {
        if (typeof ntValue === "undefined")
            return

        if (ntTopic.startsWith(item_topic)) {
            // suffix only
            let topic = ntTopic.replace(item_topic + "/", "")

            update(topic, ntValue)
            valid = true
        }
    }

    Connections {
        target: topicStore

        function onConnected(conn) {
            if (conn) {
                for (var i = 0; i < topics.length; ++i) {
                    let suffix = "/" + topics[i]

                    topicStore.forceUpdate(item_topic + suffix)
                }
            }
        }
    }

    Component.onCompleted: {
        topicStore.topicUpdate.connect(updateTopic)

        item_topic = model.topic

        for (var i = 0; i < topics.length; ++i) {
            topicStore.subscribe(item_topic + "/" + topics[i])
        }
    }

    Component.onDestruction: {
        if (topicStore !== null) {
            topicStore.topicUpdate.disconnect(updateTopic)

            for (var i = 0; i < topics.length; ++i) {
                topicStore.unsubscribe(item_topic + "/" + topics[i])
            }
        }
    }

    onItem_topicChanged: {
        for (var i = 0; i < topics.length; ++i) {
            let suffix = "/" + topics[i]
            topicStore.unsubscribe(topic + suffix)
            topicStore.subscribe(item_topic + suffix)

            topicStore.forceUpdate(item_topic + suffix)
        }

        model.topic = item_topic
    }
}
