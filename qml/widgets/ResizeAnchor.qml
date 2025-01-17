import QtQuick 2.15
import QtQuick.Controls 2.15

import QFRCDashboard 1.0
import QFDFlags 1.0

Rectangle {
    id: resizeAnchor
    property int margin
    property int divisor: 14

    required property int direction

    property alias mouseArea: mouseArea

    property bool hasLeft: direction & Qt.LeftEdge
    property bool hasRight: direction & Qt.RightEdge
    property bool hasTop: direction & Qt.TopEdge
    property bool hasBottom: direction & Qt.BottomEdge

    property bool horiz: hasLeft || hasRight
    property bool vert: hasTop || hasBottom

    color: "transparent"

    anchors {
        left: !hasRight ? parent.left : undefined
        right: !hasLeft ? parent.right : undefined
        top: !hasBottom ? parent.top : undefined
        bottom: !hasTop ? parent.bottom : undefined

        leftMargin: horiz ? 0 : margin
        rightMargin: horiz ? 0 : margin
        topMargin: vert ? 0 : margin
        bottomMargin: vert ? 0 : margin
    }

    function redoMargin() {
        margin = Math.min(parent.width, parent.height) / divisor

        anchors.leftMargin = horiz ? 0 : margin
        anchors.rightMargin = horiz ? 0 : margin
        anchors.topMargin = vert ? 0 : margin
        anchors.bottomMargin = vert ? 0 : margin

        if (vert)
            height = margin
        if (horiz)
            width = margin
    }

    Component.onCompleted: {
        redoMargin()
        parent.onWidthChanged.connect(redoMargin)
        parent.onHeightChanged.connect(redoMargin)
    }

    MouseArea {
        id: mouseArea
        z: 1

        anchors.fill: parent
        propagateComposedEvents: true

        cursorShape: {
            if (horiz && vert) {
                if ((hasLeft && hasTop) || (hasRight && hasBottom)) {
                    return Qt.SizeFDiagCursor
                } else {
                    return Qt.SizeBDiagCursor
                }
            } else if (horiz) {
                return Qt.SizeHorCursor
            } else {
                return Qt.SizeVerCursor
            }
        }

        drag.target: parent
        drag.axis: {
            if (horiz && vert) {
                return Drag.XAndYAxis
            } else if (horiz) {
                return Drag.XAxis
            } else if (vert) {
                return Drag.YAxis
            }
        }

        onMouseXChanged: {
            if (drag.active) {
                if (hasRight) {
                    parent.parent.width = parent.parent.width + mouseX
                } else if (hasLeft) {
                    parent.parent.width = parent.parent.width - mouseX
                    parent.parent.x = parent.parent.x + mouseX
                }

                resizeBegin(rect.Drag)
            }
        }

        onMouseYChanged: {
            if (drag.active) {
                if (hasBottom) {
                    parent.parent.height = parent.parent.height + mouseY
                } else if (hasTop) {
                    parent.parent.height = parent.parent.height - mouseY
                    parent.parent.y = parent.parent.y + mouseY
                }

                resizeBegin(rect.Drag)
            }
        }
    }
}
