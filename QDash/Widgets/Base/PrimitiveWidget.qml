// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import Carboxyl.Clover

BaseWidget {
    id: widget

    property string oldTopic
    property string trueTopic: topic + suffix
    property string suffix: ""
    property bool readOnly: false

    // Define this in your widget
    function update(value) {
        console.error("PrimitiveWidget's update function should NEVER be called. "
                      + "If this is the case, you likely forgot to define the update function in your widget.")
    }

    function setValue(value) {
        if (!readOnly) {
            valid = false
            TopicStore.setValue(trueTopic, value)
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
        item_topic = model.topic
        oldTopic = trueTopic
        TopicStore.subscribe(trueTopic, update)

        item_topicChanged.connect(() => {
                                      model.topic = item_topic

                                      TopicStore.unsubscribe(oldTopic, update)
                                      TopicStore.subscribe(trueTopic, update)

                                      oldTopic = trueTopic
                                  })
    }

    Component.onDestruction: {
        if (TopicStore !== null) {
            TopicStore.unsubscribe(trueTopic, update)
        }
    }
}
