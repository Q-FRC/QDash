// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Items

/**
 * Qt Quick tests for AcceleratedShape.
 *
 * AcceleratedShape is a thin wrapper around QtQuick.Shapes.Shape that enables
 * GPU layer, 8x multi-sampling, smooth rendering, and vendor extensions.
 * These tests verify that the expected defaults are applied after creation.
 */
TestCase {
    name: "AcceleratedShape"
    width: 100
    height: 100

    Component {
        id: shapeComp
        AcceleratedShape {}
    }

    // -----------------------------------------------------------------------
    // Instantiation
    // -----------------------------------------------------------------------

    function test_instantiatesWithoutError() {
        var shape = createTemporaryObject(shapeComp, testCase)
        verify(shape !== null)
    }

    // -----------------------------------------------------------------------
    // Layer properties (GPU acceleration)
    // -----------------------------------------------------------------------

    function test_layerIsEnabled() {
        var shape = createTemporaryObject(shapeComp, testCase)
        verify(shape.layer.enabled)
    }

    function test_layerSamplesIsEight() {
        var shape = createTemporaryObject(shapeComp, testCase)
        compare(shape.layer.samples, 8)
    }

    function test_layerSmoothIsTrue() {
        var shape = createTemporaryObject(shapeComp, testCase)
        verify(shape.layer.smooth)
    }

    // -----------------------------------------------------------------------
    // Shape properties
    // -----------------------------------------------------------------------

    function test_vendorExtensionsEnabled() {
        var shape = createTemporaryObject(shapeComp, testCase)
        verify(shape.vendorExtensionsEnabled)
    }
}
