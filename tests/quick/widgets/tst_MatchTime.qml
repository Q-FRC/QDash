// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Widgets.Primitive

/**
 * Qt Quick tests for the MatchTime widget.
 *
 * Notable behaviour differences from other PrimitiveWidget subclasses:
 *   - update(value) does NOT set `connected = true`.  It only updates the
 *     internal `txt.value` to Math.ceil(value), which drives the displayed
 *     MM:SS string and colour.
 *
 * Because `txt` is an internal item (no objectName), the tests verify:
 *   1. Public property defaults.
 *   2. The unique "no connected-flag" behaviour of update().
 *   3. The MM:SS formatting expression as a standalone JavaScript check so
 *      that the formula is exercised without needing to access internals.
 */
TestCase {
    name: "MatchTime"
    width: 400
    height: 300

    Component {
        id: matchTimeComp
        MatchTime {}
    }

    // -----------------------------------------------------------------------
    // Default property values
    // -----------------------------------------------------------------------

    function test_defaultFontSize() {
        var w = createTemporaryObject(matchTimeComp, testCase)
        compare(w.fontSize, 100)
    }

    function test_defaultWarningColor() {
        var w = createTemporaryObject(matchTimeComp, testCase)
        compare(w.warningColor, Qt.color("yellow"))
    }

    function test_defaultConnectedIsFalse() {
        var w = createTemporaryObject(matchTimeComp, testCase)
        verify(!w.connected)
    }

    // -----------------------------------------------------------------------
    // update() does NOT set connected
    // -----------------------------------------------------------------------

    function test_updateDoesNotSetConnected() {
        var w = createTemporaryObject(matchTimeComp, testCase)
        w.update(90)                        // call update with a time value
        verify(!w.connected)                // still false — unique to MatchTime
    }

    // -----------------------------------------------------------------------
    // MM:SS formatting expression (pure logic, no internal item access)
    // -----------------------------------------------------------------------

    function mmss(totalSeconds) {
        var v = Math.ceil(totalSeconds)
        return Math.floor(v / 60) + ":" +
               String((v % 60).toFixed(0)).padStart(2, '0')
    }

    function test_format_zeroSeconds() {
        compare(mmss(0), "0:00")
    }

    function test_format_belowOneMinute() {
        // 45 s → "0:45"
        compare(mmss(45), "0:45")
    }

    function test_format_exactlyOneMinute() {
        // 60 s → "1:00"
        compare(mmss(60), "1:00")
    }

    function test_format_minuteAndSeconds() {
        // 65 s → "1:05"
        compare(mmss(65), "1:05")
    }

    function test_format_twoMinutes() {
        // 130 s → "2:10"
        compare(mmss(130), "2:10")
    }

    function test_format_fractionalCeiled() {
        // 4.1 s → Math.ceil(4.1) = 5 → "0:05"
        compare(mmss(4.1), "0:05")
    }

    function test_format_singleDigitSecondPadded() {
        // 61 s → "1:01"
        compare(mmss(61), "1:01")
    }
}
