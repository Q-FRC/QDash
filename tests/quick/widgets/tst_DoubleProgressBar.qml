// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "DoubleProgressBarTest"
    when: windowShown

    Component {
        id: barComp
        DoubleProgressBar {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(barComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "DoubleProgressBar failed to instantiate")
        compare(w.fontSize,    20)
        compare(w.numTicks,    5)
        compare(w.lowerBound,  0.0)
        compare(w.upperBound,  100.0)
        compare(w.vertical,    false)
        compare(w.connected,   false)
    }

    function test_updateSetsConnected() {
        var w = createWidget()
        verify(w !== null)
        w.update(75.0)
        compare(w.connected, true)
    }

    function test_propertiesCanBeSet() {
        var w = createWidget()
        verify(w !== null)
        w.lowerBound = -10.0
        w.upperBound = 200.0
        w.numTicks   = 10
        w.vertical   = true
        compare(w.lowerBound, -10.0)
        compare(w.upperBound, 200.0)
        compare(w.numTicks,    10)
        compare(w.vertical,    true)
    }

    function test_suffixProperty() {
        var w = createWidget()
        verify(w !== null)
        w.suffix = " m/s"
        compare(w.suffix, " m/s")
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        w.item_topic = "/test/bar"
        ntHelper.publishDouble("/test/bar", 50.0)
        tryCompare(w, "connected", true, 2000)
    }
}
