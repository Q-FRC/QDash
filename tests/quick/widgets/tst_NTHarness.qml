// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

/**
 * tst_NTHarness: end-to-end topic-subscription tests.
 *
 * Each test:
 *  1. Creates the widget.
 *  2. Assigns a unique NT topic via item_topic so that PrimitiveWidget
 *     re-subscribes to the new topic through the real TopicStore.
 *  3. Publishes a value to that topic using the TestNTHelper (backed by the
 *     isolated nt::NetworkTableInstance created in tst_Widgets.cpp).
 *  4. Waits up to 2 s for the asynchronous NT callback chain to fire and set
 *     widget.connected = true.
 */
TestCase {
    id: root
    name: "NTHarnessTest"
    when: windowShown

    // ---- BoolWidget ----

    Component { id: boolComp; BoolWidget  { width: 184; height: 134 } }

    function test_boolWidget_topicSubscription() {
        var w = createTemporaryObject(boolComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myBool"
        ntHelper.publishBool("/nt/harness/myBool", true)
        tryCompare(w, "connected", true, 2000)
    }

    // ---- ColorWidget ----

    Component { id: colorComp; ColorWidget { width: 184; height: 134 } }

    function test_colorWidget_topicSubscription() {
        var w = createTemporaryObject(colorComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myColor"
        ntHelper.publishBool("/nt/harness/myColor", false)
        tryCompare(w, "connected", true, 2000)
        compare(w.itemValue, false)
    }

    // ---- DoubleWidget ----

    Component { id: doubleComp; DoubleWidget { width: 184; height: 134 } }

    function test_doubleWidget_topicSubscription() {
        var w = createTemporaryObject(doubleComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myDouble"
        ntHelper.publishDouble("/nt/harness/myDouble", 42.5)
        tryCompare(w, "connected", true, 2000)
    }

    // ---- DoubleGaugeWidget ----

    Component { id: gaugeComp; DoubleGaugeWidget { width: 184; height: 134 } }

    function test_doubleGaugeWidget_topicSubscription() {
        var w = createTemporaryObject(gaugeComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myGauge"
        ntHelper.publishDouble("/nt/harness/myGauge", 80.0)
        tryCompare(w, "connected", true, 2000)
    }

    // ---- DoubleProgressBar ----

    Component { id: barComp; DoubleProgressBar { width: 184; height: 134 } }

    function test_doubleProgressBar_topicSubscription() {
        var w = createTemporaryObject(barComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myBar"
        ntHelper.publishDouble("/nt/harness/myBar", 55.0)
        tryCompare(w, "connected", true, 2000)
    }

    // ---- DoubleDisplay ----

    Component { id: displayComp; DoubleDisplay { width: 184; height: 134 } }

    function test_doubleDisplay_topicSubscription() {
        var w = createTemporaryObject(displayComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myDisplay"
        ntHelper.publishDouble("/nt/harness/myDisplay", 3.14)
        tryCompare(w, "connected", true, 2000)
    }

    // ---- StringDisplay ----

    Component { id: strComp; StringDisplay { width: 184; height: 134 } }

    function test_stringDisplay_topicSubscription() {
        var w = createTemporaryObject(strComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myString"
        ntHelper.publishString("/nt/harness/myString", "hello world")
        tryCompare(w, "connected", true, 2000)
    }

    // ---- PhaseShift ----

    Component { id: phaseComp; PhaseShift { width: 184; height: 134 } }

    function test_phaseShift_topicSubscription() {
        var w = createTemporaryObject(phaseComp, root)
        verify(w !== null)
        w.item_topic = "/nt/harness/myPhase"
        ntHelper.publishDouble("/nt/harness/myPhase", 95.0)
        tryCompare(w, "time", 95, 2000)
    }
}

