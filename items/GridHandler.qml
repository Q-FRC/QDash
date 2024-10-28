import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6

import QFRCDashboard

Repeater {
    id: rep
    model: grid.rows * grid.columns

    signal resizeBegin(var drag, var valid)
    signal resizeEnd(var drag, var valid)

    signal fullCancelDrag()
    signal intersectionCheck(var drag, var source)

    function checkIntersects(drag, source) {
        intersected = []

        intersectionCheck(drag, source)

        calcRectangle(drag)
        fullCancelDrag()
    }

    function recalcValid() {
        let valid = true

        for (let i = 0; i < validResize.length; ++i) {
            if (!validResize[i]) valid = false
        }
        return valid
    }

    function beginResize(drag) {
        let valid = recalcValid()

        resizeBegin(drag, valid)
        drag.source.caught = valid
    }

    function endResize(drag) {
        let valid = recalcValid()

        drag.source.caught = valid
        intersected = []
        resizeEnd(drag, valid)

        calcRectangle(drag)
    }

    property list<bool> validResize

    Component.onCompleted: {
        for (let i = 0; i < model; ++i) {
            validResize[i] = true
        }
    }

    property list<rect> intersected

    function occupied() {
        for (let i = 0; i < model; ++i) {
            if (itemAt(i).isOccupied()) {
                return Qt.point(i / grid.columns, i % grid.columns)
            }
        }

        return Qt.point(-1, -1)
    }

    function calcRectangle(drag) {
        let rows = []
        let cols = []

        for (let i = 0; i < intersected.length; ++i) {
            let r = intersected[i]
            if (r === Qt.rect(0, 0, 0, 0)) continue;

            rows.push(r.y)
            cols.push(r.x)
        }

        let minRow = Math.min(...rows)
        let maxRow = Math.max(...rows)
        let minCol = Math.min(...cols)
        let maxCol = Math.max(...cols)

        let height = maxRow - minRow + 1
        let width = maxCol - minCol + 1

        drag.source.Layout.rowSpan = grid.rows + 1
        drag.source.Layout.columnSpan = grid.columns + 1
        drag.source.Layout.row = grid.rows + 1
        drag.source.Layout.column = grid.columns + 1
        drag.source.update()

        drag.source.mrowSpan = height
        drag.source.mcolumnSpan = width
        drag.source.mrow = minRow
        drag.source.mcolumn = minCol

        drag.source.Layout.rowSpan = height
        drag.source.Layout.columnSpan = width
        drag.source.Layout.column = minCol
        drag.source.Layout.row = minRow
    }

    delegate: Rectangle {
        id: delRect
        required property int modelData

        color: Constants.palette.bg
        z: 2
        border {
            color: "gray"
            width: 2
        }

        Layout.row: modelData / grid.columns
        Layout.column: modelData % grid.columns

        Layout.preferredWidth: grid.prefWidth(this)
        Layout.preferredHeight: grid.prefHeight(this)

        function isOccupied() {
            // @disable-check M126
            return border.color == "#90ee90"
        }

        function resetBorder() {
            border.color = "gray"
            border.width = 2
        }

        function valueInRange(value, min, max) {
            return (value >= min) && (value <= max)
        }

        function intersects(A, B) {
            let xOverlap = valueInRange(A.x, B.x, B.x + B.width - 1) ||
                valueInRange(B.x, A.x, A.x + A.width - 1);

            let yOverlap = valueInRange(A.y, B.y, B.y + B.height - 1) ||
                valueInRange(B.y, A.y, A.y + A.height - 1);

            return xOverlap && yOverlap;
        }

        function rectDrag(drag) {
            let sourceRect = Qt.rect(drag.source.x, drag.source.y, drag.source.width, drag.source.height)
            let myRect = Qt.rect(x, y, width, height)

            if (intersects(sourceRect, myRect)) {
                drop.dragEnter(drag)
            } else {
                rep.validResize[modelData] = true
                resetBorder()
            }
        }

        function endDrag(drag) {
            let sourceRect = Qt.rect(drag.source.x, drag.source.y, drag.source.width, drag.source.height)
            let myRect = Qt.rect(x, y, width, height)

            if (intersects(sourceRect, myRect)) {
                rep.intersected[modelData] = Qt.rect(Layout.column, Layout.row, 1, 1)
            }

            rep.validResize[modelData] = true
            resetBorder()
        }

        function checkIntersect(drag, source) {
            let myRect = Qt.rect(Layout.column, Layout.row, 1, 1)

            if (intersects(source, myRect)) {
                let realSource = Qt.rect(drag.source.mcolumn, drag.source.mrow, drag.source.mcolumnSpan, drag.source.mrowSpan)
                rep.validResize[modelData] = drop.validSpotRect(realSource)

                rep.intersected[modelData] = Qt.rect(Layout.column, Layout.row, 1, 1)

                border.color = rep.recalcValid() ? "light green" : "red"
                border.width = 5
            } else {
                rep.validResize[modelData] = true
                resetBorder()
            }

            let valid = rep.recalcValid()

            drag.source.caught = valid
        }

        Component.onCompleted: {
            rep.resizeBegin.connect(rectDrag)
            rep.resizeEnd.connect(endDrag)
            rep.intersectionCheck.connect(checkIntersect)
            rep.fullCancelDrag.connect(resetBorder)
        }

        DropArea {
            id: drop
            anchors.fill: parent

            function validSpotRect(source) {
                return !twm.cellOccupied(parent.Layout.row, parent.Layout.column, parent.Layout.rowSpan, parent.Layout.columnSpan, source)
            }

            function validSpot(drag) {
                let source = Qt.rect(drag.source.mcolumn, drag.source.mrow, drag.source.mcolumnSpan, drag.source.mrowSpan)
                return validSpotRect(source)
            }

            function dragEnter(drag, valid) {
                parent.border.width = 5

                rep.validResize[modelData] = validSpot(drag)

                parent.border.color = rep.recalcValid() ? "light green" : "red"
            }

            onEntered: (drag) => {
                           {
                               parent.border.width = 5

                               let source = Qt.rect(parent.Layout.column, parent.Layout.row, drag.source.mcolumnSpan, drag.source.mrowSpan)
                               rep.intersectionCheck(drag, source)
                           }
                       }

            onExited: {
                rep.validResize[modelData] = true
                resetBorder()
            }

            onDropped: (drag) => {
                           let source = Qt.rect(parent.Layout.column, parent.Layout.row, drag.source.mcolumnSpan, drag.source.mrowSpan)
                           rep.checkIntersects(drag, source)
                           drag.accept()

                           rep.fullCancelDrag()
                       }
        }
    }
}
