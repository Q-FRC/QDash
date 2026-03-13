// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 6.4
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic as B
import QtQuick.Layouts 2.15

import Carboxyl.Clover
import QDash.Items
import QDash.Config

Repeater {
    model: [Qt.RightEdge, Qt.LeftEdge, Qt.TopEdge, Qt.BottomEdge, Qt.RightEdge
        | Qt.TopEdge, Qt.RightEdge | Qt.BottomEdge, Qt.LeftEdge
        | Qt.TopEdge, Qt.LeftEdge | Qt.BottomEdge]

    ResizeAnchor {
        required property int modelData

        control: widget
        direction: modelData
        z: horiz && vert ? 26 : 24

        mouseArea.onPressed: mouse => {
                                 if (mouse.button === Qt.RightButton) {
                                     drag.target = null
                                     widget.openContextMenu()
                                 } else if (mouse.button === Qt.LeftButton) {
                                     startResize()
                                 }
                             }
        mouseArea.onReleased: mouse => {
                                  if (!tab.isCopying
                                      && mouse.button === Qt.LeftButton) {
                                      if (grid.validResize(widget.width,
                                                           widget.height,
                                                           widget.x, widget.y,
                                                           row, column,
                                                           rowSpan, colSpan)) {

                                          let newSize = grid.getRect(
                                              widget.x, widget.y, widget.width,
                                              widget.height)

                                          widget.mrowSpan = newSize.height
                                          widget.mcolumnSpan = newSize.width
                                          widget.mrow = newSize.y
                                          widget.mcolumn = newSize.x

                                          widget.fixSize()
                                      } else {
                                          widget.animateBacksize()
                                      }

                                      resizeActive = false
                                      grid.resetValid()

                                      widget.z = 3
                                  }
                              }
    }
}
