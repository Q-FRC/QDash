// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtTest
import QDash.Widgets.Primitive

TestCase {
    id: root
    name: "ColorWidgetTest"
    when: windowShown

    Component {
        id: colorComp
        ColorWidget {
            width: 184
            height: 134
        }
    }

    function createWidget() {
        return createTemporaryObject(colorComp, root)
    }

    function test_defaultProperties() {
        var w = createWidget()
        verify(w !== null, "ColorWidget failed to instantiate")
        compare(w.falseColor.toString().toUpperCase(), "#FF0000")
        compare(w.trueColor.toString().toUpperCase(),  "#00FF00")
        compare(w.shape, "Rectangle")
        compare(w.connected, false)
    }

    function test_updateSetsConnected() {
        var w = createWidget()
        verify(w !== null)
        w.update(true)
        compare(w.connected, true)
        compare(w.itemValue, true)
    }

    function test_updateFalseValue() {
        var w = createWidget()
        verify(w !== null)
        w.update(true)
        w.update(false)
        compare(w.itemValue, false)
    }

    function test_colorPropertiesCanBeChanged() {
        var w = createWidget()
        verify(w !== null)
        w.falseColor = "#0000FF"
        compare(w.falseColor.toString().toUpperCase(), "#0000FF")
        w.trueColor = "#FFFF00"
        compare(w.trueColor.toString().toUpperCase(), "#FFFF00")
    }

    function test_shapeChoicesExist() {
        var w = createWidget()
        verify(w !== null)
        verify(w.shapeChoices.length > 0)
        verify(w.shapeChoices.indexOf("Rectangle") >= 0)
        verify(w.shapeChoices.indexOf("Circle") >= 0)
    }

    function test_ntTopicSubscription() {
        var w = createWidget()
        verify(w !== null)
        w.item_topic = "/test/color"
        ntHelper.publishBool("/test/color", true)
        tryCompare(w, "connected", true, 2000)
    }
}
