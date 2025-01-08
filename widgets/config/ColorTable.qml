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

        font.pixelSize: 15 * Constants.scalar
    }

    HorizontalHeaderView {
        id: horizontalHeader

        Layout.minimumWidth: parent.width - 90 * Constants.scalar
        Layout.row: 0
        Layout.column: 1

        Layout.fillWidth: true

        delegate: Rectangle {
            id: delegate

            required property var model

            // Qt6: add cellPadding (and font etc) as public API in headerview
            readonly property real cellPadding: 8

            implicitWidth: text.implicitWidth + (cellPadding * 2)
            implicitHeight: Math.max(horizontalHeader.height, text.implicitHeight + (cellPadding * 2))
            color: "#f6f6f6"
            border.color: "#e4e4e4"

            Label {
                id: text
                font.pixelSize: 18 * Constants.scalar

                text: delegate.model[horizontalHeader.textRole]
                width: delegate.width
                height: delegate.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "#ff26282a"
            }
        }

        Layout.minimumHeight: 30 * Constants.scalar
        Layout.columnSpan: 2

        syncView: tbl
        clip: true
    }

    TableView {
        Layout.minimumWidth: parent.width - 90 * Constants.scalar
        Layout.minimumHeight: 40 * Constants.scalar
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
            Layout.minimumHeight = 42 * Constants.scalar * tblModel.rowCount()
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
            implicitHeight: 40 * Constants.scalar

            color: current ? "blue" : "black"

            Text {
                font.pixelSize: 15 * Constants.scalar
                anchors.centerIn: parent
                text: display
                color: Constants.palette.text
            }

            TableView.editDelegate: TextField {
                font.pixelSize: 15 * Constants.scalar
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
        Layout.fillWidth: true
        uniformCellSizes: true
        Button {
            Layout.alignment: Qt.AlignLeft
            text: "Add"
            font.pixelSize: 18 * Constants.scalar

            onClicked: {
                tblModel.add("", "")
                tbl.resetHeight()
            }
        }

        Button {
            Layout.alignment: Qt.AlignLeft
            text: "Delete"
            font.pixelSize: 18 * Constants.scalar

            onClicked: {
                tblModel.remove(tbl.currentRow)
                tbl.resetHeight()
            }
        }

        Button {
            Layout.alignment: Qt.AlignLeft
            text: "Copy Color"
            font.pixelSize: 18 * Constants.scalar

            onClicked: colorDialog.open()
        }
    }
}
