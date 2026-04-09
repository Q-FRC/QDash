// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Widgets.Primitive

/**
 * Qt Quick tests for the StringDisplay widget.
 *
 * StringDisplay renders a string NT value as a fitted, optionally wrapped
 * Text item.  update(value) sets connected = true and updates the internal
 * txt.text.  The `wrap` property controls the text wrap mode.
 */
TestCase {
    name: "StringDisplay"
    width: 400
    height: 300

    Component {
        id: stringDisplayComp
        StringDisplay {}
    }

    // -----------------------------------------------------------------------
    // Default property values
    // -----------------------------------------------------------------------

    function test_defaultFontSize() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        compare(w.fontSize, 100)
    }

    function test_defaultWrapIsTrue() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        verify(w.wrap)
    }

    function test_defaultConnectedIsFalse() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        verify(!w.connected)
    }

    // -----------------------------------------------------------------------
    // update() side effects
    // -----------------------------------------------------------------------

    function test_updateSetsConnectedTrue() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        w.update("hello")
        verify(w.connected)
    }

    function test_updateEmptyStringSetsConnected() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        w.update("")
        verify(w.connected)
    }

    // -----------------------------------------------------------------------
    // wrap property
    // -----------------------------------------------------------------------

    function test_wrapCanBeSetToFalse() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        w.wrap = false
        verify(!w.wrap)
    }

    function test_wrapRoundTrip() {
        var w = createTemporaryObject(stringDisplayComp, testCase)
        w.wrap = false
        w.wrap = true
        verify(w.wrap)
    }
}
