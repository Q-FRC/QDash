// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "PhaseShiftTest"
    when: windowShown

    Component {
        id: phaseComp
        PhaseShift {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(phaseComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "PhaseShift failed to instantiate")
        compare(w.fontSize,          100)
        compare(w.warningThreshold,  3)
        compare(w.flashInterval,     500)
        // default time (no update yet)
        compare(w.time, 130)
    }

    function test_updatePhaseTransitionShift() {
        // time > 130  =>  active = Both
        var w = createWidget()
        verify(w !== null)
        w.update(140)
        compare(w.active, PhaseShift.Both)
    }

    function test_updatePhasePhase1() {
        // 105 < time <= 130  =>  active = firstActive (default Red)
        var w = createWidget()
        verify(w !== null)
        w.update(110)
        compare(w.active, PhaseShift.Red)
    }

    function test_updatePhasePhase2() {
        // 80 < time <= 105  =>  active = secondActive (default Blue)
        var w = createWidget()
        verify(w !== null)
        w.update(90)
        compare(w.active, PhaseShift.Blue)
    }

    function test_updatePhaseEndgame() {
        // time <= 30  =>  active = Both
        var w = createWidget()
        verify(w !== null)
        w.update(25)
        compare(w.active, PhaseShift.Both)
    }

    function test_warningThresholdProperty() {
        var w = createWidget()
        verify(w !== null)
        w.warningThreshold = 5
        compare(w.warningThreshold, 5)
    }

    function test_fontSizeProperty() {
        var w = createWidget()
        verify(w !== null)
        w.fontSize = 80
        compare(w.fontSize, 80)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        w.item_topic = "/test/phase"
        ntHelper.publishDouble("/test/phase", 120.0)
        tryCompare(w, "time", 120, 2000)
    }
}
