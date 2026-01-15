// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import Carboxyl.Clover

BaseWidget {
    id: widget

    property string trueTopic: topic + suffix
    property string suffix: ""
    property bool readOnly: false

    // Define this in your widget
    function update(value) {}

    function setValue(value) {
        if (!readOnly) {
            valid = false
            TopicStore.setValue(trueTopic, value)
        }
    }

    // TODO: find a way to prevent every single widget from getting this call?
    function updateTopic(ntTopic, ntValue) {
        if (typeof ntValue === "undefined")
            return

        if (ntTopic === trueTopic) {
            update(ntValue)

            valid = true
            if (QDashSettings.disableWidgets)
                connected = true
        }
    }

    Connections {
        target: TopicStore

        function onConnected(conn) {
            if (conn) {
                TopicStore.forceUpdate(widget.trueTopic)
            } else {
                if (QDashSettings.disableWidgets)
                    widget.connected = false
                widget.valid = false
            }
        }
    }

    Component.onCompleted: {
        TopicStore.topicUpdate.connect(updateTopic)
        TopicStore.subscribe(model.topic + suffix)

        item_topic = model.topic
        TopicStore.forceUpdate(item_topic)
    }

    Component.onDestruction: {
        if (TopicStore !== null) {
            TopicStore.topicUpdate.disconnect(updateTopic)
            TopicStore.unsubscribe(trueTopic)
        }
    }

    onItem_topicChanged: {
        TopicStore.unsubscribe(topic + suffix)
        TopicStore.subscribe(item_topic + suffix)
        model.topic = item_topic

        TopicStore.forceUpdate(item_topic + suffix)
    }
}
