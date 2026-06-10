// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: resizeAnchor

    required property var control
    required property int direction
    property int divisor: 14

    // Edges
    property bool hasBottom: direction & Qt.BottomEdge
    property bool hasLeft: direction & Qt.LeftEdge
    property bool hasRight: direction & Qt.RightEdge
    property bool hasTop: direction & Qt.TopEdge

    property bool horiz: hasLeft || hasRight
    property bool vert: hasTop || hasBottom

    // only enable resize anchor if it's not at the edge of the grid,
    // AND resizing isn't possible in that direction.
    readonly property bool _singleColumn: control.mcolumnSpan === 1
    readonly property bool _leftEdge: hasLeft && control.mcolumn === 0 && _singleColumn
    readonly property bool _rightEdge: hasRight && (control.mcolumn + control.mcolumnSpan) >= tab.cols && _singleColumn

    readonly property bool _singleRow: control.mrowSpan === 1
    readonly property bool _topEdge: hasTop && control.mrow === 0 && _singleRow
    readonly property bool _bottomEdge: hasBottom && (control.mrow + control.mrowSpan) >= tab.rows && _singleRow

    readonly property bool isAtEdge: _leftEdge || _rightEdge || _topEdge || _bottomEdge

    // etc
    property int margin: Math.min(control.width, control.height) / divisor
    property alias mouseArea: mouseArea

    width: (hasLeft || hasRight) ? margin : parent.width - (margin * 2)
    height: (hasTop || hasBottom) ? margin : parent.height - (margin * 2)

    enabled: !isAtEdge
    mouseArea.enabled: !isAtEdge

    z: 20

    anchors {
        left: !hasRight ? parent.left : undefined
        right: !hasLeft ? parent.right : undefined
        bottom: !hasTop ? parent.bottom : undefined
        top: !hasBottom ? parent.top : undefined

        leftMargin: horiz ? 0 : margin
        rightMargin: horiz ? 0 : margin
        bottomMargin: vert ? 0 : margin
        topMargin: vert ? 0 : margin
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
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

        drag.axis: {
            if (horiz && vert) {
                return Drag.XAndYAxis
            } else if (horiz) {
                return Drag.XAxis
            } else if (vert) {
                return Drag.YAxis
            }
        }

        drag.target: parent
        hoverEnabled: true
        propagateComposedEvents: true
        z: 26

        onMouseXChanged: {
            if (drag.active) {
                let newWidth = control.width
                let newX = control.x

                if (hasRight) {
                    newWidth += mouseX
                } else if (hasLeft) {
                    newWidth -= mouseX
                    newX += mouseX
                }

                if (newWidth >= control.minWidth) {
                    control.width = newWidth
                    control.x = newX
                } else {
                    if (hasLeft) {
                        let diff = control.minWidth - control.width
                        if (Math.abs(diff) < 0.5)
                            diff = 0
                        control.x -= diff
                    }
                    control.width = control.minWidth
                }
            }
        }

        onMouseYChanged: {
            if (drag.active) {
                let newHeight = control.height
                let newY = control.y

                if (hasBottom) {
                    newHeight += mouseY
                } else if (hasTop) {
                    newHeight -= mouseY
                    newY += mouseY
                }

                if (newHeight >= control.minHeight) {
                    control.height = newHeight
                    control.y = newY
                } else {
                    if (hasLeft) {
                        let diff = control.minHeight - control.height
                        if (Math.abs(diff) < 0.5)
                            diff = 0
                        control.y -= diff
                    }
                    control.height = control.minHeight
                }
            }
        }

        onPressed: mouse => {
            // simple clicks that don't do anything get passed to the titleField
            if (hasTop && mouse.y > margin / 2) {
                mouse.accepted = false
            } else {
                mouse.accepted = true
            }
        }
    }
}
