// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import Carboxyl.Clover

BaseWidget {
    id: widget

    property list<string> topics

    // Define this in your widget
    // this takes in the suffix only
    function update(topic, value) {}

    function setValue(topic, value) {
        valid = false
        TopicStore.setValue(item_topic + "/" + topic, value)
    }

    function updateTopic(ntTopic, ntValue) {
        if (typeof ntValue === "undefined")
            return

        if (ntTopic.startsWith(item_topic)) {
            // suffix only
            let topic = ntTopic.replace(item_topic + "/", "")

            update(topic, ntValue)
            valid = true

            if (QDashSettings.disableWidgets)
                connected = true
        }
    }

    Connections {
        target: TopicStore

        function onConnected(conn) {
            if (conn) {
                for (var i = 0; i < topics.length; ++i) {
                    let suffix = "/" + topics[i]

                    TopicStore.forceUpdate(item_topic + suffix)
                }
            } else {
                widget.valid = false
                if (QDashSettings.disableWidgets)
                    widget.connected = false
            }
        }
    }

    Component.onCompleted: {
        TopicStore.topicUpdate.connect(updateTopic)

        item_topic = model.topic

        for (var i = 0; i < topics.length; ++i) {
            TopicStore.subscribe(item_topic + "/" + topics[i])
        }
    }

    Component.onDestruction: {
        if (TopicStore !== null) {
            TopicStore.topicUpdate.disconnect(updateTopic)

            for (var i = 0; i < topics.length; ++i) {
                TopicStore.unsubscribe(item_topic + "/" + topics[i])
            }
        }
    }

    onItem_topicChanged: {
        for (var i = 0; i < topics.length; ++i) {
            let suffix = "/" + topics[i]
            TopicStore.unsubscribe(topic + suffix)
            TopicStore.subscribe(item_topic + suffix)

            TopicStore.forceUpdate(item_topic + suffix)
        }

        model.topic = item_topic
    }
}
