import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: resizeAnchor
    property int margin
    property int divisor: 14

    enabled: true

    required property int direction

    property alias mouseArea: mouseArea

    property bool hasLeft: direction & Qt.LeftEdge
    property bool hasRight: direction & Qt.RightEdge
    property bool hasTop: direction & Qt.TopEdge
    property bool hasBottom: direction & Qt.BottomEdge

    property bool horiz: hasLeft || hasRight
    property bool vert: hasTop || hasBottom
    z: 20

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
                let newWidth = parent.parent.width
                let newX = parent.parent.x

                if (hasRight) {
                    newWidth += mouseX
                } else if (hasLeft) {
                    newWidth -= mouseX
                    newX += mouseX
                }

                if (newWidth >= parent.parent.minWidth) {
                    parent.parent.width = newWidth
                    parent.parent.x = newX
                } else {
                    if (hasLeft) {
                        let diff = parent.parent.minWidth - parent.parent.width
                        if (Math.abs(diff) < 0.5)
                            diff = 0
                        parent.parent.x -= diff
                    }
                    parent.parent.width = parent.parent.minWidth
                }
            }
        }

        onMouseYChanged: {
            if (drag.active) {
                let newHeight = parent.parent.height
                let newY = parent.parent.y

                if (hasBottom) {
                    newHeight += mouseY
                } else if (hasTop) {
                    newHeight -= mouseY
                    newY += mouseY
                }

                if (newHeight >= parent.parent.minHeight) {
                    parent.parent.height = newHeight
                    parent.parent.y = newY
                } else {
                    if (hasLeft) {
                        let diff = parent.parent.minHeight - parent.parent.height
                        if (Math.abs(diff) < 0.5)
                            diff = 0
                        parent.parent.y -= diff
                    }
                    parent.parent.height = parent.parent.minHeight
                }
            }
        }
    }
}
