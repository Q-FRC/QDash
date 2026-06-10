// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QDash.Core
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 2.15

AcceleratedShape {
    id: shape

    property color itemColor
    property string itemShape
    readonly property real radius: Math.min(width, height) / 2

    // inject path into shape data
    data: [shapeLoader.item]

    anchors {
        left: parent.left
        right: parent.right
        bottom: parent.bottom
        top: titleField.bottom

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

    Component {
        id: triangleComp

        ShapePath {
            fillColor: shape.itemColor
            startX: 0
            startY: shape.height
            strokeColor: shape.itemColor
            strokeWidth: 1

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
            fillColor: shape.itemColor
            startX: 0
            startY: 0
            strokeColor: shape.itemColor
            strokeWidth: 1

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
            fillColor: shape.itemColor
            startX: shape.width / 2
            startY: 0
            strokeColor: shape.itemColor
            strokeWidth: 1

            PathArc {
                radiusX: shape.radius
                radiusY: shape.radius
                useLargeArc: true
                x: shape.width / 2
                y: shape.width < shape.height ? shape.width : shape.height
            }

            PathArc {
                radiusX: shape.radius
                radiusY: shape.radius
                useLargeArc: true
                x: shape.width / 2
                y: 0
            }
        }
    }
}
