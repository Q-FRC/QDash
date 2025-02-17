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

        GridHandler {
            id: rep
        }
    }
}
