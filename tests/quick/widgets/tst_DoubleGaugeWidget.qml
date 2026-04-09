// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "DoubleGaugeWidgetTest"
    when: windowShown

    Component {
        id: gaugeComp
        DoubleGaugeWidget {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(gaugeComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "DoubleGaugeWidget failed to instantiate")
        compare(w.fontSize,    20)
        compare(w.ticks,       15)
        compare(w.startAngle, -135.0)
        compare(w.endAngle,    135.0)
        compare(w.min,         0.0)
        compare(w.max,         100.0)
        compare(w.connected,   false)
    }

    function test_updateSetsConnected() {
        var w = createWidget()
        verify(w !== null)
        w.update(50.0)
        compare(w.connected, true)
    }

    function test_propertiesCanBeSet() {
        var w = createWidget()
        verify(w !== null)
        w.ticks      = 10
        w.startAngle = -90.0
        w.endAngle   = 90.0
        w.min        = -100.0
        w.max        = 100.0
        compare(w.ticks,      10)
        compare(w.startAngle, -90.0)
        compare(w.endAngle,    90.0)
        compare(w.min,        -100.0)
        compare(w.max,         100.0)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        w.item_topic = "/test/gauge"
        ntHelper.publishDouble("/test/gauge", 75.0)
        tryCompare(w, "connected", true, 2000)
    }
}
