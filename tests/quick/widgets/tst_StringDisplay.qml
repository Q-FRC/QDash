// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "StringDisplayTest"
    when: windowShown

    Component {
        id: strComp
        StringDisplay {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(strComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "StringDisplay failed to instantiate")
        compare(w.fontSize, 100)
        compare(w.wrap,     true)
        compare(w.connected, false)
    }

    function test_updateSetsConnected() {
        var w = createWidget()
        verify(w !== null)
        w.update("Hello World")
        compare(w.connected, true)
    }

    function test_fontSizeProperty() {
        var w = createWidget()
        verify(w !== null)
        w.fontSize = 50
        compare(w.fontSize, 50)
    }

    function test_wrapProperty() {
        var w = createWidget()
        verify(w !== null)
        w.wrap = false
        compare(w.wrap, false)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        w.item_topic = "/test/string"
        ntHelper.publishString("/test/string", "test value")
        tryCompare(w, "connected", true, 2000)
    }
}
