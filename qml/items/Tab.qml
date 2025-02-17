import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6

import Qt.labs.qmlmodels

import QFRCDashboard

Rectangle {
    id: tab
    width: Constants.width
    height: Constants.height
    color: Constants.palette.bg

    signal copying(point mousePos)
    signal dropped(point mousePos)

    signal storeWidget(var w)

    property bool isCopying: false

    property var latestWidget

    property alias lastOpSuccessful: grid.currentOpValid

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        property point mouseCoordinates: Qt.point(0, 0)

        onPositionChanged: mouse => {
                               mouseCoordinates = Qt.point(mouse.x, mouse.y)
                               if (isCopying) {
                                   copying(mouseCoordinates)
                               }
                           }

        onClicked: {
            if (copying) {
                isCopying = false
                dropped(mouseCoordinates)
            }
        }
    }

    TabWidgetsModel {
        id: twm

        rows: model.rows
        cols: model.cols
    }

    Component.onCompleted: {
        if (model.widgets === null)
            model.widgets = twm
        else {
            twm.setEqualTo(model.widgets)
            model.widgets = twm
        }
    }

    function copy(idx) {
        let w = twm.copy(idx)
        storeWidget(w)
    }

    function paste(w) {
        twm.add(w)
        isCopying = true
        copying(mouseArea.mouseCoordinates)
    }

    function removeLatest() {
        twm.removeLatest()
    }

    function add(title, topic, type) {
        twm.add(title, topic, type)
    }

    function setName(name) {
        model.title = name
    }

    function name() {
        return model.title
    }

    function setSize(r, c) {
        model.rows = r
        model.cols = c

        twm.rows = r
        twm.cols = c

        grid.rows = r
        grid.columns = c
    }

    function cols() {
        return model.cols
    }

    function rows() {
        return model.rows
    }

    Rectangle {
        id: validRect
        color: "transparent"
        z: 5

        border {
            width: 5
            color: "transparent"
        }
    }

    GridLayout {
        id: grid
        rows: model.rows
        columns: model.cols

        anchors.fill: parent

        columnSpacing: 0
        rowSpacing: 0
        z: 4

        property bool currentOpValid: false

        function validResize(width, height, row, column, rowSpan, colSpan) {
            let size = getSize(width, height)

            let newRowSpan = size.height
            let newColSpan = size.width

            let ignore = Qt.rect(column, row, colSpan, rowSpan)

            let valid = !twm.cellOccupied(row, column, newRowSpan,
                                          newColSpan, ignore)

            validRect.x = column * colWidth()
            validRect.y = row * rowWidth()
            validRect.width = newColSpan * colWidth()
            validRect.height = newRowSpan * rowWidth()

            validRect.border.color = valid ? "lightgreen" : "red"
            currentOpValid = valid
            return valid
        }

        function validSpot(x, y, row, column, rowSpan, colSpan, round) {
            let point = getPoint(x, y, round)

            let newRow = point.y
            let newCol = point.x

            let ignore = Qt.rect(column, row, colSpan, rowSpan)

            let valid = !twm.cellOccupied(newRow, newCol, rowSpan,
                                          colSpan, ignore)

            validRect.x = newCol * colWidth()
            validRect.y = newRow * rowWidth()
            validRect.width = colSpan * colWidth()
            validRect.height = rowSpan * rowWidth()

            validRect.border.color = valid ? "lightgreen" : "red"
            currentOpValid = valid
            return valid
        }

        function resetValid() {
            validRect.border.color = "transparent"
        }

        function getPoint(x, y, round) {
            var newRow, newCol
            if (round) {
                newRow = Math.round(y / rowWidth())
                newCol = Math.round(x / colWidth())
            } else {
                newRow = Math.floor(y / rowWidth())
                newCol = Math.floor(x / colWidth())
            }

            if (newRow < 0)
                newRow = 0
            if (newRow >= rows)
                newRow = rows - 1

            if (newCol < 0)
                newCol = 0
            if (newCol >= columns)
                newCol = columns - 1

            return Qt.point(newCol, newRow)
        }

        function getSize(width, height) {
            let newRows = Math.round(height / rowWidth())
            let newCols = Math.round(width / colWidth())

            if (newRows < 1)
                newRows = 1

            if (newCols < 1)
                newCols = 1

            return Qt.size(newCols, newRows)
        }

        function colWidth() {
            return grid.width / grid.columns
        }

        function rowWidth() {
            return grid.height / grid.rows
        }

        function prefWidth(item) {
            return colWidth() * item.Layout.columnSpan
        }

        function prefHeight(item) {
            return rowWidth() * item.Layout.rowSpan
        }

        Repeater {
            model: twm

            delegate: DelegateChooser {
                id: chooser
                role: "type"
                DelegateChoice {
                    roleValue: "int"
                    IntWidget {}
                }
                DelegateChoice {
                    roleValue: "string"
                    TextWidget {}
                }

                DelegateChoice {
                    roleValue: "double"
                    DoubleWidget {}
                }

                DelegateChoice {
                    roleValue: "bool"
                    BoolWidget {}
                }

                DelegateChoice {
                    roleValue: "dial"
                    IntDialWidget {}
                }

                DelegateChoice {
                    roleValue: "doubleDial"
                    DoubleDialWidget {}
                }

                DelegateChoice {
                    roleValue: "color"
                    ColorWidget {}
                }

                DelegateChoice {
                    roleValue: "FMSInfo"
                    FMSInfo {}
                }

                DelegateChoice {
                    roleValue: "Field2d"
                    Field2d {}
                }

                DelegateChoice {
                    roleValue: "Command"
                    Command {}
                }

                DelegateChoice {
                    roleValue: "String Chooser"
                    StringChooser {}
                }

                DelegateChoice {
                    roleValue: "camera"
                    CameraView {}
                }

                DelegateChoice {
                    roleValue: "enum"
                    EnumWidget {}
                }

                DelegateChoice {
                    roleValue: "errors"
                    ErrorsWidget {}
                }

                DelegateChoice {
                    roleValue: "reef"
                    ReefDisplay {}
                }

                DelegateChoice {
                    roleValue: "doubleGauge"
                    DoubleGaugeWidget {}
                }

                DelegateChoice {
                    roleValue: "gauge"
                    IntGaugeWidget {}
                }

                DelegateChoice {
                    roleValue: "doubleBar"
                    DoubleProgressBar {}
                }

                DelegateChoice {
                    roleValue: "doubleDisplay"
                    DoubleDisplay {}
                }

                DelegateChoice {
                    roleValue: "intDisplay"
                    IntDisplay {}
                }

                DelegateChoice {
                    roleValue: "matchTime"
                    MatchTime {}
                }

                DelegateChoice {
                    roleValue: "textDisplay"
                    StringDisplay {}
                }
            }
        }

        Repeater {
            model: grid.rows * grid.columns

            Rectangle {
                color: "transparent"

                border {
                    color: "gray"
                    width: 1
                }

                Layout.row: modelData / grid.columns
                Layout.column: modelData % grid.columns

                Layout.preferredWidth: grid.prefWidth(this)
                Layout.preferredHeight: grid.prefHeight(this)
            }
        }
    }
}
