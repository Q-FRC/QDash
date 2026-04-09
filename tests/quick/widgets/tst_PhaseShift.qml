// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Widgets.Primitive

/**
 * Qt Quick tests for PhaseShift widget — specifically the updatePhase()
 * function which contains the game-phase computation logic.
 *
 * Phase boundaries (all exclusive-lower, inclusive-upper boundaries on `time`):
 *   time > 130  → Transition  active = Both,        remainingTime = time % 130
 *   time > 105  → Phase 1     active = firstActive,  remainingTime = time % 105
 *   time > 80   → Phase 2     active = secondActive, remainingTime = time % 80
 *   time > 55   → Phase 3     active = firstActive,  remainingTime = time % 55
 *   time > 30   → Phase 4     active = secondActive, remainingTime = time % 30
 *   time <= 30  → Endgame     active = Both,         remainingTime = time,
 *                             needsWarning = false
 *
 * Default alliance order (from defaults in PhaseShift.qml):
 *   firstActive  = PhaseShift.Red  (0)
 *   secondActive = PhaseShift.Blue (1)
 *
 * After updateGSM('R'):
 *   firstActive  = PhaseShift.Blue (1)
 *   secondActive = PhaseShift.Red  (0)
 */
TestCase {
    name: "PhaseShift"
    width: 400
    height: 300

    Component {
        id: phaseShiftComp
        PhaseShift {}
    }

    // -----------------------------------------------------------------------
    // Default values
    // -----------------------------------------------------------------------

    function test_defaultActive() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        compare(w.active, 2)        // PhaseShift.Both = 2
    }

    function test_defaultFirstActive() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        compare(w.firstActive, 0)   // PhaseShift.Red = 0
    }

    function test_defaultSecondActive() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        compare(w.secondActive, 1)  // PhaseShift.Blue = 1
    }

    function test_defaultRedAlliance() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        compare(w.redAlliance, false)
    }

    function test_defaultNeedsWarning() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        compare(w.needsWarning, false)
    }

    function test_defaultWarningColor() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        compare(w.warningColor, Qt.color("yellow"))
    }

    // -----------------------------------------------------------------------
    // update() — raw-time ceiling
    // -----------------------------------------------------------------------

    function test_updateCeilsRawTime() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(5.3)
        // Math.ceil(5.3) = 6 → endgame (time <= 30)
        compare(w.time, 6)
    }

    // -----------------------------------------------------------------------
    // Phase boundaries
    // -----------------------------------------------------------------------

    function test_transition_aboveOneThirty() {
        // time > 130 → Transition, active = Both (2)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(150)
        compare(w.active, 2)                    // Both
        compare(w.remainingTime, 150 % 130)     // 20
        verify(w.needsWarning)
    }

    function test_phaseOne_between106and130() {
        // 105 < time <= 130 → Phase 1, firstActive = Red (0)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(120)
        compare(w.active, 0)                    // Red
        compare(w.remainingTime, 120 % 105)     // 15
        verify(w.needsWarning)
    }

    function test_phaseTwo_between81and105() {
        // 80 < time <= 105 → Phase 2, secondActive = Blue (1)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(90)
        compare(w.active, 1)                    // Blue
        compare(w.remainingTime, 90 % 80)       // 10
        verify(w.needsWarning)
    }

    function test_phaseThree_between56and80() {
        // 55 < time <= 80 → Phase 3, firstActive = Red (0)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(60)
        compare(w.active, 0)                    // Red
        compare(w.remainingTime, 60 % 55)       // 5
        verify(w.needsWarning)
    }

    function test_phaseFour_between31and55() {
        // 30 < time <= 55 → Phase 4, secondActive = Blue (1)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(40)
        compare(w.active, 1)                    // Blue
        compare(w.remainingTime, 40 % 30)       // 10
        verify(w.needsWarning)
    }

    function test_endgame_atThirty() {
        // time = 30 → Endgame (not > 30), active = Both (2), needsWarning = false
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(30)
        compare(w.active, 2)                    // Both
        compare(w.remainingTime, 30)
        verify(!w.needsWarning)
    }

    function test_endgame_belowThirty() {
        // time < 30 → Endgame, active = Both (2)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(15)
        compare(w.active, 2)                    // Both
        compare(w.remainingTime, 15)
        verify(!w.needsWarning)
    }

    // -----------------------------------------------------------------------
    // updateGSM — alliance swap
    // -----------------------------------------------------------------------

    function test_updateGSM_R_swapsAlliances() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.updateGSM('R')
        compare(w.firstActive,  1)  // Blue
        compare(w.secondActive, 0)  // Red
    }

    function test_updateGSM_B_restoresDefaults() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.updateGSM('R')            // swap first
        w.updateGSM('B')            // restore
        compare(w.firstActive,  0)  // Red
        compare(w.secondActive, 1)  // Blue
    }

    // -----------------------------------------------------------------------
    // hubActive derived property
    // -----------------------------------------------------------------------

    function test_hubActive_isTrueWhenBothAlliances() {
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.update(150)               // → active = Both (2)
        verify(w.hubActive)
    }

    function test_hubActive_blueAllianceActiveForBluePlayer() {
        // non-red player (redAlliance = false) → hub is active when active = Blue (1)
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.redAlliance = false
        w.update(90)                // → secondActive = Blue (1) → active = Blue
        verify(w.hubActive)
    }

    function test_hubActive_falseWhenWrongAlliance() {
        // non-red player, active = Red → hub NOT active
        var w = createTemporaryObject(phaseShiftComp, testCase)
        w.redAlliance = false
        w.update(120)               // → firstActive = Red (0) → active = Red
        verify(!w.hubActive)
    }
}
