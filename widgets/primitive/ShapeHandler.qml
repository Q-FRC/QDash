import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 2.15

import QFRCDashboard

Shape {
    id: shape

    function setColor() {
        circle.setColor()
        rect.setColor()
        tri.setColor()
    }

    property color itemColor
    property string itemShape

    anchors {
        top: titleField.bottom
        left: parent.left
        right: parent.right
        bottom: parent.bottom

        margins: 10
    }

    ShapePath {
        id: tri
        strokeWidth: 1

        function setColor() {
            strokeColor = itemShape === "Triangle" ? (shape.itemColor) : "transparent"
            fillColor = itemShape === "Triangle" ? (shape.itemColor) : "transparent"
            update()
        }

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

    ShapePath {
        id: rect
        strokeWidth: 1

        function setColor() {
            strokeColor = itemShape === "Rectangle" ? (shape.itemColor) : "transparent"
            fillColor = itemShape === "Rectangle" ? (shape.itemColor) : "transparent"
            update()
        }

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

    ShapePath {
        id: circle
        strokeWidth: 1

        function setColor() {
            strokeColor = itemShape === "Circle" ? (shape.itemColor) : "transparent"
            fillColor = itemShape === "Circle" ? (shape.itemColor) : "transparent"
            update()
        }

        startX: shape.width / 2
        startY: 0

        PathArc {
            x: shape.width / 2
            y: shape.width < shape.height ? shape.width : shape.height

            radiusX: Math.min(shape.width, shape.height) / 2
            radiusY: Math.min(shape.width, shape.height) / 2

            useLargeArc: true
        }

        PathArc {
            x: shape.width / 2
            y: 0

            radiusX: Math.min(shape.width, shape.height) / 2
            radiusY: Math.min(shape.width, shape.height) / 2

            useLargeArc: true
        }
    }
}
