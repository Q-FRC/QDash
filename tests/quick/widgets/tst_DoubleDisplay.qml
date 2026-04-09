// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "DoubleDisplayTest"
    when: windowShown

    Component {
        id: displayComp
        DoubleDisplay {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(displayComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "DoubleDisplay failed to instantiate")
        compare(w.maxFontSize, 100)
        compare(w.decimals,    2)
        compare(w.connected,   false)
    }

    function test_updateSetsConnected() {
        var w = createWidget()
        verify(w !== null)
        w.update(3.14)
        compare(w.connected, true)
    }

    function test_propertiesCanBeSet() {
        var w = createWidget()
        verify(w !== null)
        w.maxFontSize = 80
        w.decimals    = 4
        compare(w.maxFontSize, 80)
        compare(w.decimals,    4)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        w.item_topic = "/test/display"
        ntHelper.publishDouble("/test/display", 99.99)
        tryCompare(w, "connected", true, 2000)
    }
}
