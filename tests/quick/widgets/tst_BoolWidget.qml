// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Widgets.Primitive

/**
 * Qt Quick tests for BoolWidget.
 *
 * BoolWidget shows a CheckBox that mirrors an NT boolean topic.
 * update(value) sets connected = true and updates the internal control.
 */
TestCase {
    name: "BoolWidget"
    width: 400
    height: 300

    Component {
        id: boolWidgetComp
        BoolWidget {}
    }

    // -----------------------------------------------------------------------
    // Default property values
    // -----------------------------------------------------------------------

    function test_defaultCheckboxSize() {
        var w = createTemporaryObject(boolWidgetComp, testCase)
        compare(w.checkboxSize, 40)
    }

    function test_defaultTitleFontSize() {
        // titleFontSize is declared in BaseWidget
        var w = createTemporaryObject(boolWidgetComp, testCase)
        compare(w.titleFontSize, 16)
    }

    function test_defaultConnectedIsFalse() {
        var w = createTemporaryObject(boolWidgetComp, testCase)
        verify(!w.connected)
    }

    function test_defaultValidIsFalse() {
        var w = createTemporaryObject(boolWidgetComp, testCase)
        verify(!w.valid)
    }

    // -----------------------------------------------------------------------
    // update() side effects
    // -----------------------------------------------------------------------

    function test_updateSetsConnectedTrue() {
        var w = createTemporaryObject(boolWidgetComp, testCase)
        w.update(true)
        verify(w.connected)
    }

    function test_updateWithFalseStillSetsConnected() {
        var w = createTemporaryObject(boolWidgetComp, testCase)
        w.update(false)
        verify(w.connected)
    }
}
