// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: resizeAnchor

    property int divisor: 14
    property int margin: Math.min(control.width, control.height) / divisor

    required property int direction
    required property var control

    property alias mouseArea: mouseArea

    property bool hasLeft: direction & Qt.LeftEdge
    property bool hasRight: direction & Qt.RightEdge
    property bool hasTop: direction & Qt.TopEdge
    property bool hasBottom: direction & Qt.BottomEdge

    property bool horiz: hasLeft || hasRight
    property bool vert: hasTop || hasBottom
    z: 20

    // only enable resize anchor if it's not at the edge of the grid,
    // AND resizing isn't possible in that direction.
    // TODO(crueter): Wtf is this formatting
    readonly property bool isAtEdge: ((hasLeft && control.mcolumn === 0 && control.mcolumnSpan === 1)
                                      || (hasTop && control.mrow === 0 && control.mrowSpan
                                          === 1) || (hasRight
                                                     && (control.mcolumn + control.mcolumnSpan) >= tab.cols && control.mcolumnSpan
                                                     === 1) || (hasBottom
                                                                && (control.mrow + control.mrowSpan) >= tab.rows && control.mrowSpan === 1))

    enabled: !isAtEdge
    mouseArea.enabled: !isAtEdge

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

    width: (hasLeft || hasRight) ? margin : parent.width - (margin * 2)
    height: (hasTop || hasBottom) ? margin : parent.height - (margin * 2)

    MouseArea {
        id: mouseArea
        z: 26

        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true

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

        onPressed: mouse => {
                       // simple clicks that don't do anything get passed to the titleField
                       if (hasTop && mouse.y > margin / 2) {
                           mouse.accepted = false
                       } else {
                           mouse.accepted = true
                       }
                   }
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
    }
}
