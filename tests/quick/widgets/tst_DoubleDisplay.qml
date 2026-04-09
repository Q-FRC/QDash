// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Widgets.Primitive

/**
 * Qt Quick tests for the DoubleDisplay widget.
 *
 * DoubleDisplay renders a floating-point NT value as a fitted Text item.
 * update(value) sets connected = true and updates the internal txt.value,
 * which is formatted with toFixed(decimals).
 *
 * Because toFixed(decimals) result is only visible on the internal `txt`
 * item (no objectName exposed), the tests cover the public properties and
 * the observable connected-flag side effect of update().
 */
TestCase {
    name: "DoubleDisplay"
    width: 400
    height: 300

    Component {
        id: doubleDisplayComp
        DoubleDisplay {}
    }

    // -----------------------------------------------------------------------
    // Default property values
    // -----------------------------------------------------------------------

    function test_defaultMaxFontSize() {
        var w = createTemporaryObject(doubleDisplayComp, testCase)
        compare(w.maxFontSize, 100)
    }

    function test_defaultDecimals() {
        var w = createTemporaryObject(doubleDisplayComp, testCase)
        compare(w.decimals, 2)
    }

    function test_defaultConnectedIsFalse() {
        var w = createTemporaryObject(doubleDisplayComp, testCase)
        verify(!w.connected)
    }

    // -----------------------------------------------------------------------
    // update() side effects
    // -----------------------------------------------------------------------

    function test_updateSetsConnectedTrue() {
        var w = createTemporaryObject(doubleDisplayComp, testCase)
        w.update(3.14159)
        verify(w.connected)
    }

    function test_updateWithZeroSetsConnected() {
        var w = createTemporaryObject(doubleDisplayComp, testCase)
        w.update(0.0)
        verify(w.connected)
    }

    // -----------------------------------------------------------------------
    // toFixed formatting expression (pure logic)
    // -----------------------------------------------------------------------

    function test_toFixed_twoDecimals() {
        compare((3.14159).toFixed(2), "3.14")
    }

    function test_toFixed_zeroDecimals() {
        compare((7.89).toFixed(0), "8")
    }

    function test_toFixed_fourDecimals() {
        compare((1.23456).toFixed(4), "1.2346")
    }
}
