
// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick.Shapes
import QtQuick

// GPU-accelerated shape with 8x sampling
// so it's smooth
Shape {
    layer.enabled: true
    layer.samples: 4
    layer.smooth: true

    vendorExtensionsEnabled: true
    Component.onCompleted: {
        if (typeof this.preferredRendererType != 'undefined') {
            this.preferredRendererType = Shape.CurveRenderer
        } else {
            logs.debug("UI", "Qt version is too old for curve renderer.")
        }
    }
}
