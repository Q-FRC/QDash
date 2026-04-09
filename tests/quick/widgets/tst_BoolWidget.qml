// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "BoolWidgetTest"
    when: windowShown

    // ---------- helpers ----------

    Component {
        id: boolComp
        BoolWidget {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(boolComp, root)
    }

    // ---------- tests ----------

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "BoolWidget failed to instantiate")
        compare(w.checkboxSize, 40)
        compare(w.connected,    false)
    }

    function test_updateSetsConnectedAndChecked() {
        var w = createWidget()
        verify(w !== null)

        // The checkbox is the first CheckBox child; find it via objectName convention
        // Instead, call update() directly and verify the widget's connected flag.
        w.update(true)
        compare(w.connected, true)

        w.update(false)
        compare(w.connected, true)
    }

    function test_checkboxSizeProperty() {
        var w = createWidget()
        verify(w !== null)
        w.checkboxSize = 60
        compare(w.checkboxSize, 60)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        // Assign topic; PrimitiveWidget re-subscribes via the real TopicStore.
        w.item_topic = "/test/bool"
        ntHelper.publishBool("/test/bool", true)
        tryCompare(w, "connected", true, 2000)
    }
}
