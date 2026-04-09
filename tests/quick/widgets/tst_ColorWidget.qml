// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Widgets.Primitive

/**
 * Qt Quick tests for ColorWidget.
 *
 * ColorWidget receives a boolean NT value and fills a configurable shape
 * with trueColor or falseColor accordingly.  Because itemValue is a public
 * property, we can verify that update() updates it correctly.
 */
TestCase {
    name: "ColorWidget"
    width: 400
    height: 300

    Component {
        id: colorWidgetComp
        ColorWidget {}
    }

    // -----------------------------------------------------------------------
    // Default property values
    // -----------------------------------------------------------------------

    function test_defaultFalseColor() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        compare(w.falseColor, Qt.color("#FF0000"))
    }

    function test_defaultTrueColor() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        compare(w.trueColor, Qt.color("#00FF00"))
    }

    function test_defaultShape() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        compare(w.shape, "Rectangle")
    }

    function test_defaultItemValueIsFalse() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        verify(!w.itemValue)
    }

    function test_shapeChoicesContainsRectangle() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        verify(w.shapeChoices.indexOf("Rectangle") >= 0)
    }

    function test_shapeChoicesContainsCircle() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        verify(w.shapeChoices.indexOf("Circle") >= 0)
    }

    function test_shapeChoicesContainsTriangle() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        verify(w.shapeChoices.indexOf("Triangle") >= 0)
    }

    // -----------------------------------------------------------------------
    // update() side effects
    // -----------------------------------------------------------------------

    function test_updateTrueSetsItemValueAndConnected() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        w.update(true)
        verify(w.itemValue)
        verify(w.connected)
    }

    function test_updateFalseSetsItemValueFalseAndConnected() {
        var w = createTemporaryObject(colorWidgetComp, testCase)
        w.update(true)      // set to true first
        w.update(false)     // then back to false
        verify(!w.itemValue)
        verify(w.connected)
    }
}
