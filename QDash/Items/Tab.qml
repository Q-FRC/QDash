import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 6.6

import Qt.labs.qmlmodels
import QtQml.Models

import QDash.Constants
import QDash.Native.Models
import QDash.Widgets.Primitive
import QDash.Widgets.Sendable
import QDash.Widgets.Misc
import QDash.Native.Helpers

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
    property int rows: model.rows
    property int cols: model.cols

    property double colWidth: tab.width / tab.cols
    property double rowHeight: tab.height / tab.rows

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

    function fakeAdd(title, topic, type) {
        twm.add(title, topic, type)
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

    // TODO: If too many rows or cols, default widgets to 2x2
    // TODO: Default bigger widgets to 2x2 or 3x2
    Repeater {
        id: grid

        property bool currentOpValid: false

        function validResize(width, height, x, y, row, column, rowSpan, colSpan) {
            let rect = getRect(x, y, width, height)

            let newRow = rect.y
            let newColumn = rect.x

            let newRowSpan = rect.height
            let newColSpan = rect.width

            let ignore = Qt.rect(column, row, colSpan, rowSpan)

            let valid = !twm.cellOccupied(newRow, newColumn, newRowSpan,
                                          newColSpan, ignore)

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

            let valid = !twm.cellOccupied(newRow, newCol, rowSpan,
                                          colSpan, ignore)

            validRect.x = newCol * colWidth
            validRect.y = newRow * rowHeight
            validRect.width = colSpan * colWidth
            validRect.height = rowSpan * rowHeight

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
            let point = getPoint(x, y, false)

            // Hacky fix for weird margins issues
            let bottomRight = getPoint(x + (width - 16),
                                       y + (height - 16), false)

            let newRows = Math.ceil(bottomRight.y - point.y + 1)
            let newCols = Math.ceil(bottomRight.x - point.x + 1)

            if (newRows < 1)
                newRows = 1

            if (newCols < 1)
                newCols = 1

            return Qt.rect(point.x, point.y, newCols, newRows)
        }

        property double colWidth: tab.colWidth
        property double rowHeight: tab.rowHeight

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

            // TODO: Physically remove it from the model if not present.
            DelegateChoice {
                roleValue: "camera"

                // TODO: generify
                delegate: Loader {
                    z: 3

                    sourceComponent: CompileDefinitions.useCameraView ? Qt.createComponent(
                                                                            "../Widgets/Misc/CameraView.qml") : nullComponent
                }
            }

            DelegateChoice {
                roleValue: "web"
                delegate: Loader {
                    z: 3

                    sourceComponent: CompileDefinitions.useWebView ? Qt.createComponent(
                                                                         "../Widgets/Misc/WebView.qml") : nullComponent
                }
            }

            DelegateChoice {
                roleValue: "colorText"
                ColorTextWidget {}
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

        Component {
            id: nullComponent
            Item {}
        }
    }

    Repeater {
        id: gridLines

        model: tab.rows * tab.cols

        Rectangle {
            color: "transparent"
            z: 2

            border {
                color: "gray"
                width: 1
            }

            width: tab.colWidth
            height: tab.rowHeight

            x: tab.colWidth * (modelData % tab.cols)
            y: tab.rowHeight * Math.floor(modelData / tab.cols)
        }
    }
}
