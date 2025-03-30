import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string trueTopic: item_topic + suffix
    property string suffix: ""

    property bool valid: true

    // Define this in your widget
    function update(value) {}

    function setValue(value) {
        valid = false
        topicStore.setValue(trueTopic, value)
    }

    function updateTopic(ntTopic, ntValue) {
        if (typeof ntValue === "undefined")
            return

        if (ntTopic === trueTopic) {
            update(ntValue)

            valid = true
        }
    }

    Connections {
        target: topicStore

        function onConnected(conn) {
            if (conn) {
                topicStore.forceUpdate(widget.trueTopic)
            }
        }
    }

    Component.onCompleted: {
        topicStore.topicUpdate.connect(updateTopic)
        item_topic = model.topic

        topicStore.subscribe(trueTopic)
    }

    Component.onDestruction: {
        if (topicStore !== null) {
            topicStore.topicUpdate.disconnect(updateTopic)
            topicStore.unsubscribe(trueTopic)
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic + suffix)
        topicStore.subscribe(trueTopic)
        model.topic = item_topic

        topicStore.forceUpdate(trueTopic)
    }
}
