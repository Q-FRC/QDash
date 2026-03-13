// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import Carboxyl.Clover

BaseWidget {
    id: widget

    property string oldTopic
    property list<string> topics
    property list<var> funcs

    // Define this in your widget
    // this takes in the suffix only
    function update(topic, value) {
        console.error(
                    "SendableWidget's update function should NEVER be called. "
                    + "If this is the case, you likely forgot to define the update function in your widget.")
    }

    function setValue(topic, value) {
        valid = false
        TopicStore.setValue(item_topic + "/" + topic, value)
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
        item_topic = model.topic

        for (var i = 0; i < topics.length; ++i) {
            let topic = topics[i]
            funcs[i] = value => update(topic, value)
            TopicStore.subscribe(item_topic + "/" + topic, funcs[i])
        }

        oldTopic = model.topic

        item_topicChanged.connect(() => {
                                      model.topic = item_topic

                                      for (var i = 0; i < topics.length; ++i) {
                                          let suffix = "/" + topics[i]
                                          let f = funcs[i]
                                          TopicStore.unsubscribe(
                                              oldTopic + suffix, f)
                                          TopicStore.subscribe(
                                              item_topic + suffix, f)
                                      }

                                      oldTopic = item_topic
                                  })
    }

    Component.onDestruction: {
        if (TopicStore !== null) {
            for (var i = 0; i < topics.length; ++i) {
                TopicStore.unsubscribe(item_topic + "/" + topics[i], funcs[i])
            }
        }
    }
}
