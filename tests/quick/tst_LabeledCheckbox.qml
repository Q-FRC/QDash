// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Config

/**
 * Qt Quick tests for LabeledCheckbox.
 *
 * The component binds to an external target object via bindTarget[bindedProperty].
 * Each test creates fresh instances through createTemporaryObject so state
 * cannot leak between test functions.
 */
TestCase {
    name: "LabeledCheckbox"
    width: 240
    height: 60

    // -----------------------------------------------------------------------
    // Shared components
    // -----------------------------------------------------------------------

    // A minimal QtObject that exposes a bool property used as the bind target.
    Component {
        id: mockTargetComp
        QtObject {
            property bool myBoolProp: false
        }
    }

    // LabeledCheckbox under test – required properties are supplied per test.
    Component {
        id: checkboxComp
        LabeledCheckbox {}
    }

    // -----------------------------------------------------------------------
    // Label binding
    // -----------------------------------------------------------------------

    function test_textMatchesLabelProperty() {
        var target = createTemporaryObject(mockTargetComp, testCase)
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Enable Feature",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        compare(cb.text, "Enable Feature")
    }

    function test_textUpdatesWhenLabelChanges() {
        var target = createTemporaryObject(mockTargetComp, testCase)
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Original",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.label = "Updated"
        compare(cb.text, "Updated")
    }

    // -----------------------------------------------------------------------
    // open() – reads bindTarget[bindedProperty] into checked
    // -----------------------------------------------------------------------

    function test_openSetsFalseWhenTargetIsFalse() {
        var target = createTemporaryObject(mockTargetComp, testCase, {myBoolProp: false})
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Test",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.checked = true   // deliberately wrong state
        cb.open()
        compare(cb.checked, false)
    }

    function test_openSetsTrueWhenTargetIsTrue() {
        var target = createTemporaryObject(mockTargetComp, testCase, {myBoolProp: true})
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Test",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.checked = false  // deliberately wrong state
        cb.open()
        compare(cb.checked, true)
    }

    // -----------------------------------------------------------------------
    // accept() – writes checked back to bindTarget[bindedProperty]
    // -----------------------------------------------------------------------

    function test_acceptWritesFalseToTarget() {
        var target = createTemporaryObject(mockTargetComp, testCase, {myBoolProp: true})
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Test",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.checked = false
        cb.accept()
        compare(target.myBoolProp, false)
    }

    function test_acceptWritesTrueToTarget() {
        var target = createTemporaryObject(mockTargetComp, testCase, {myBoolProp: false})
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Test",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.checked = true
        cb.accept()
        compare(target.myBoolProp, true)
    }

    // -----------------------------------------------------------------------
    // open() / accept() round-trip
    // -----------------------------------------------------------------------

    function test_openThenAcceptPreservesValue() {
        var target = createTemporaryObject(mockTargetComp, testCase, {myBoolProp: true})
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Test",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.open()
        cb.accept()
        compare(target.myBoolProp, true)
    }

    function test_openThenToggleThenAcceptUpdatesTarget() {
        var target = createTemporaryObject(mockTargetComp, testCase, {myBoolProp: false})
        var cb = createTemporaryObject(checkboxComp, testCase, {
            label: "Test",
            bindedProperty: "myBoolProp",
            bindTarget: target
        })
        cb.open()
        cb.checked = !cb.checked   // toggle
        cb.accept()
        compare(target.myBoolProp, true)
    }
}
