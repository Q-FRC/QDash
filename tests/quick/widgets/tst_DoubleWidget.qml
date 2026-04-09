// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "DoubleWidgetTest"
    when: windowShown

    Component {
        id: doubleComp
        DoubleWidget {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(doubleComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "DoubleWidget failed to instantiate")
        compare(w.stepSize,    0.1)
        compare(w.fontSize,    20)
        compare(w.lowerBound, -100000.0)
        compare(w.upperBound,  100000.0)
        compare(w.connected,   false)
    }

    function test_updateSetsConnected() {
        var w = createWidget()
        verify(w !== null)
        w.update(42.5)
        compare(w.connected, true)
    }

    function test_updateValueReflectedInSpinBox() {
        var w = createWidget()
        verify(w !== null)
        w.update(12.34)
        // find the DoubleSpinBox child by searching for it
        var spin = findChild(w, "spin")
        if (spin !== null) {
            // value may be quantized by decimals; just verify it's close
            verify(Math.abs(spin.value - 12.34) < 0.01)
        }
    }

    function test_propertiesCanBeSet() {
        var w = createWidget()
        verify(w !== null)
        w.stepSize   = 0.5
        w.fontSize   = 24
        w.lowerBound = -50.0
        w.upperBound = 50.0
        compare(w.stepSize,    0.5)
        compare(w.fontSize,    24)
        compare(w.lowerBound, -50.0)
        compare(w.upperBound,  50.0)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        // Assign a topic and publish via the real NT instance
        w.item_topic = "/test/double"
        ntHelper.publishDouble("/test/double", 7.5)
        tryCompare(w, "connected", true, 2000)
    }
}
