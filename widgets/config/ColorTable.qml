import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Shapes 2.15

import QFRCDashboard

ColumnLayout {
    required property string label

    /** what property to bind to */
    required property string bindedProperty

    /** the target to bind the property to */
    required property var bindTarget


    function open() {
        tblModel.toList(bindTarget[bindedProperty])
        tbl.resetHeight()
    }

    function accept() {
        bindTarget[bindedProperty] = tblModel.asList()
    }

    id: col

    spacing: 0

    ColorDialog {
        id: colorDialog

        onAccepted: platformHelper.copy(selectedColor)
    }

    Text {
        id: floatingLabel
        text: label
        color: Constants.palette.text

        font.pixelSize: 15
    }

    HorizontalHeaderView {
        Layout.minimumWidth: parent.width - 90
        Layout.row: 0
        Layout.column: 1

        Layout.fillWidth: true

        Layout.minimumHeight: 30
        Layout.columnSpan: 2
        id: horizontalHeader
        syncView: tbl
        clip: true
    }

    TableView {
        Layout.minimumWidth: parent.width - 90
        Layout.minimumHeight: 40
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        id: tbl

        Layout.fillWidth: true

        Layout.row: 1
        Layout.column: 1

        Layout.rowSpan: 3

        interactive: false

        function resetHeight() {
            Layout.minimumHeight = 42 * tblModel.rowCount()
        }

        model: MapModel {
            id: tblModel
            valueName: "Color"
        }

        selectionModel: ItemSelectionModel {}
        selectionBehavior: TableView.SelectRows
        selectionMode: TableView.SingleSelection

        delegate: Rectangle {
            border {
                color: Constants.palette.text
                width: 2
            }

            required property bool selected
            required property bool current

            implicitWidth: tbl.width / 2
            implicitHeight: 40

            color: current ? "blue" : "black"

            Text {
                font.pixelSize: 15
                anchors.centerIn: parent
                text: display
                color: Constants.palette.text
            }

            TableView.editDelegate: TextField {
                font.pixelSize: 15
                anchors.fill: parent
                text: display
                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter
                Component.onCompleted: selectAll()

                TableView.onCommit: {
                    display = text
                }
            }
        }
    }

    RowLayout {
        Button {
            Layout.alignment: Qt.AlignLeft
            text: "Add"

            onClicked: {
                tblModel.add("", "")
                tbl.resetHeight()
            }
        }

        Button {
            Layout.alignment: Qt.AlignLeft
            text: "Delete"

            onClicked: {
                tblModel.remove(tbl.currentRow)
                tbl.resetHeight()
            }
        }

        Button {
            Layout.alignment: Qt.AlignLeft
            text: "Copy Color"

            onClicked: colorDialog.open()
        }
    }
}
