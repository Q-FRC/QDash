// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 2.15

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base

AcceleratedShape {
    id: shape

    property color itemColor
    property string itemShape

    readonly property real radius: Math.min(width, height) / 2

    anchors {
        top: titleField.bottom
        left: parent.left
        right: parent.right
        bottom: parent.bottom
        margins: 10
    }

    // shape selector
    Loader {
        id: shapeLoader
        sourceComponent: {
            if (itemShape === "Triangle")
                return triangleComp
            if (itemShape === "Rectangle")
                return rectangleComp
            if (itemShape === "Circle")
                return circleComp
            return null
        }
    }

    // inject path into shape data
    data: [shapeLoader.item]

    Component {
        id: triangleComp
        ShapePath {
            strokeWidth: 1
            strokeColor: shape.itemColor
            fillColor: shape.itemColor

            startX: 0
            startY: shape.height

            PathLine {
                x: shape.width / 2
                y: 0
            }
            PathLine {
                x: shape.width
                y: shape.height
            }
            PathLine {
                x: 0
                y: shape.height
            }
        }
    }

    Component {
        id: rectangleComp
        ShapePath {
            strokeWidth: 1
            strokeColor: shape.itemColor
            fillColor: shape.itemColor

            startX: 0
            startY: 0

            PathLine {
                x: shape.width
                y: 0
            }
            PathLine {
                x: shape.width
                y: shape.height
            }
            PathLine {
                x: 0
                y: shape.height
            }
            PathLine {
                x: 0
                y: 0
            }
        }
    }

    Component {
        id: circleComp
        ShapePath {
            strokeWidth: 1
            strokeColor: shape.itemColor
            fillColor: shape.itemColor

            startX: shape.width / 2
            startY: 0

            PathArc {
                x: shape.width / 2
                y: shape.width < shape.height ? shape.width : shape.height
                radiusX: shape.radius
                radiusY: shape.radius
                useLargeArc: true
            }

            PathArc {
                x: shape.width / 2
                y: 0
                radiusX: shape.radius
                radiusY: shape.radius
                useLargeArc: true
            }
        }
    }
}
