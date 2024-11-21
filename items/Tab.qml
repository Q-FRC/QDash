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

    property var latestWidget

    property alias gridHandler: rep

    signal copying(point mousePos)
    signal dropped(point mousePos)

    signal storeWidget(var w)

    property bool isCopying: false

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        property point mouseCoordinates: Qt.point(0, 0)

        onPositionChanged: mouse =>
                           {
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
        let w = twm.copy(idx);
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

    function addCamera(name, source, urls) {
        twm.addCamera(name, source, urls)
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

    GridLayout {
        id: grid
        rows: model.rows
        columns: model.cols

        anchors.fill: parent

        columnSpacing: 0
        rowSpacing: 0

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
                    IntWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }
                DelegateChoice {
                    roleValue: "string"
                    TextWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "double"
                    DoubleWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "bool"
                    BoolWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "dial"
                    IntDialWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "doubleDial"
                    DoubleDialWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "color"
                    ColorWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "FMSInfo"
                    FMSInfo {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "Field2d"
                    Field2d {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "Command"
                    Command {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "String Chooser"
                    StringChooser {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "camera"
                    CameraView {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "enum"
                    EnumWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }

                DelegateChoice {
                    roleValue: "errors"
                    ErrorsWidget {
                        Layout.row: model.row
                        Layout.column: model.column
                        Layout.rowSpan: model.rowSpan
                        Layout.columnSpan: model.colSpan

                        Layout.margins: 8

                        Layout.preferredWidth: grid.prefWidth(this)
                        Layout.preferredHeight: grid.prefHeight(this)
                    }
                }
            }
        }

        GridHandler {
            id: rep
        }
    }
}
