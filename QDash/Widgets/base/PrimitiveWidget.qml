// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

BaseWidget {
    id: widget

    property string oldTopic
    property bool readOnly: false
    property string suffix: ""
    property string trueTopic: topic + suffix

    function _subscribe() {
        if (enabled && trueTopic) {
            TopicStore.subscribe(trueTopic, update)
            TopicStore.forceUpdate(trueTopic)
        }
    }

    function _unsubscribe() {
        if (oldTopic) {
            TopicStore.unsubscribe(oldTopic, update)
        }
    }

    function setValue(value) {
        if (!readOnly) {
            valid = false
            TopicStore.setValue(trueTopic, value)
        }
    }

    // Define this in your widget
    function update(value) {
        console.error("PrimitiveWidget's update function should NEVER be called. "
                      + "If this is the case, you likely forgot to define the update function in your widget.")
    }

    Component.onCompleted: {
        item_topic = model.topic
        oldTopic = trueTopic

        if (enabled)
            _subscribe()

        item_topicChanged.connect(() => {
            model.topic = item_topic

            if (enabled)
                _unsubscribe()

            oldTopic = trueTopic

            if (enabled)
                _subscribe()
        })
    }
    Component.onDestruction: {
        if (TopicStore !== null)
            _unsubscribe()
    }
    onEnabledChanged: {
        if (enabled)
            _subscribe()
        else
            _unsubscribe()
    }

    Connections {
        function onConnected(conn) {
            if (conn) {
                TopicStore.forceUpdate(widget.trueTopic)
            } else {
                if (QDashSettings.disableWidgets)
                    widget.connected = false
                widget.valid = false
            }
        }

        target: TopicStore
    }
}
