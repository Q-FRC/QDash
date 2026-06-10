// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Backend.Models
import QDash.Widgets

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

Rectangle {
    id: tab

    property double colWidth: tab.width / tab.cols
    property int cols: model.cols
    property bool isCopying: false
    property alias lastOpSuccessful: grid.currentOpValid
    property var latestWidget
    property double rowHeight: tab.height / tab.rows
    property int rows: model.rows
    property rect topicViewRect
    readonly property TabWidgetsModel twm: model.widgets

    signal copying(point mousePos)
    signal dropped(point mousePos)
    signal storeWidget(var w)

    function add(title, topic, type) {
        twm.add(title, topic, type)
    }

    function copy(idx) {
        let w = twm.copy(idx)
        storeWidget(w)
    }

    function fakeAdd(title, topic, type) {
        twm.add(title, topic, type)
        isCopying = true
        copying(mouseArea.mouseCoordinates)
    }

    function name() {
        return model.title
    }

    function paste(w) {
        twm.add(w)
        isCopying = true
        copying(mouseArea.mouseCoordinates)
    }

    function removeLatest() {
        twm.removeLatest()
    }

    function setName(name) {
        model.title = name
    }

    function setSize(r, c) {
        model.rows = r
        model.cols = c

        twm.rows = r
        twm.cols = c
    }

    color: "transparent"

    Loader {
        active: QDashSettings.hannahMontanaMode
        asynchronous: true

        sourceComponent: Component {
            Image {
                fillMode: Image.PreserveAspectCrop
                height: tab.height - 2
                source: "qrc:/qt/qml/QDash/Main/hannahmontana.jpg"
                width: tab.width - 2
                x: 1
                y: 1
            }
        }
    }

    MouseArea {
        id: mouseArea

        property point mouseCoordinates: Qt.point(0, 0)

        anchors.fill: parent
        hoverEnabled: true
        z: isCopying ? 10 : 0

        onClicked: {
            if (isCopying) {
                isCopying = false
                dropped(mouseCoordinates)
            }
        }
        onPositionChanged: mouse => {
            mouseCoordinates = Qt.point(mouse.x, mouse.y);

            // TODO(crueter): This code sucks. Rewrite it again please
            if (isCopying) {
                copying(mouseCoordinates)
            }
        }
    }

    Rectangle {
        id: validRect

        color: "transparent"
        radius: 15
        z: 5

        border {
            color: "transparent"
            width: 4
        }
    }

    // TODO: If too many rows or cols, default widgets to 2x2
    // TODO: Default bigger widgets to 2x2 or 3x2
    Repeater {
        id: grid

        property double colWidth: tab.colWidth
        property bool currentOpValid: false
        property double rowHeight: tab.rowHeight

        function getPoint(x, y, round) {
            var newRow, newCol
            if (round) {
                newRow = Math.round(y / rowHeight)
                newCol = Math.round(x / colWidth)
            } else {
                newRow = Math.floor(y / rowHeight)
                newCol = Math.floor(x / colWidth)
            }

            if (newRow < 0)
                newRow = 0
            if (newRow >= rows)
                newRow = rows - 1

            if (newCol < 0)
                newCol = 0
            if (newCol >= tab.cols)
                newCol = tab.cols - 1

            return Qt.point(newCol, newRow)
        }

        function getRect(x, y, width, height) {
            let point = getPoint(x, y, false);

            // Hacky fix for weird margins issues
            let bottomRight = getPoint(x + (width - 16), y + (height - 16), false)

            let newRows = Math.ceil(bottomRight.y - point.y + 1)
            let newCols = Math.ceil(bottomRight.x - point.x + 1)

            if (newRows < 1)
                newRows = 1

            if (newCols < 1)
                newCols = 1

            return Qt.rect(point.x, point.y, newCols, newRows)
        }

        function resetValid() {
            validRect.border.color = "transparent"
        }

        function validResize(width, height, x, y, row, column, rowSpan, colSpan) {
            let rect = getRect(x, y, width, height)

            let newRow = rect.y
            let newColumn = rect.x

            let newRowSpan = rect.height
            let newColSpan = rect.width

            let ignore = Qt.rect(column, row, colSpan, rowSpan)

            let valid = !twm.cellOccupied(newRow, newColumn, newRowSpan, newColSpan, ignore)

            validRect.x = newColumn * colWidth
            validRect.y = newRow * rowHeight
            validRect.width = newColSpan * colWidth
            validRect.height = newRowSpan * rowHeight

            validRect.border.color = valid ? "lightgreen" : "red"
            currentOpValid = valid
            return valid
        }

        function validSpot(x, y, row, column, rowSpan, colSpan, round) {
            let point = getPoint(x, y, round)

            let newRow = point.y
            let newCol = point.x

            let ignore = Qt.rect(column, row, colSpan, rowSpan)

            let valid = !twm.cellOccupied(newRow, newCol, rowSpan, colSpan, ignore)

            validRect.x = newCol * colWidth
            validRect.y = newRow * rowHeight
            validRect.width = colSpan * colWidth
            validRect.height = rowSpan * rowHeight

            validRect.border.color = valid ? "lightgreen" : "red"
            currentOpValid = valid
            return valid
        }

        model: twm

        delegate: WidgetDelegate {}
    }

    Canvas {
        property int c: tab.cols
        property real colW: tab.colWidth
        property int r: tab.rows
        property real rowH: tab.rowHeight

        enabled: parent.visible
        renderStrategy: Canvas.Cooperative
        visible: parent.visible
        z: 2

        onColWChanged: requestPaint()
        onPaint: {
            let ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height);

            // TODO(crueter): make this change based on theme
            ctx.strokeStyle = "gray"
            ctx.lineWidth = 1
            ctx.beginPath()

            for (var i = 1; i < c; i++) {
                let x = Math.round(i * colW)
                ctx.moveTo(x, 0)
                ctx.lineTo(x, height)
            }

            for (var j = 1; j < r; j++) {
                let y = Math.round(j * rowH)
                ctx.moveTo(0, y)
                ctx.lineTo(width, y)
            }

            ctx.stroke()
        }
        onRowHChanged: requestPaint()
        onVisibleChanged: if (visible)
            requestPaint()

        anchors {
            fill: parent
            margins: 1
        }
    }
}
