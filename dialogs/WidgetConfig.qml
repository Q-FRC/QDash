import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import Qt.labs.qmlmodels
import QFRCDashboard

Dialog {
    id: widgetConf

    anchors.centerIn: parent

    width: parent.width / 1.6
    height: parent.height - 60
    background: Rectangle {
        color: Constants.palette.dialogBg
    }

    ColorDialog {
        id: colorDialog
    }

    property var item

    function openUp(item) {
        lm.clear()
        for (var p in item) {
            if (p.startsWith("item_") && typeof item[p] !== "function") {
                let sub = p.substr(5)
                let typeName = MetaObjectHelper.typeName(item, p)
                var choices = []
                let obj = {}

                if (typeName === "QVariant") {
                    choices = item[sub + "Choices"]

                    typeName = "list"
                }

                else if (typeName === "QVariantList") {
                    obj["valueType"] = item[sub + "ValueType"]
                    obj["valueName"] = item[sub + "ValueName"]
                }

                else if (typeName === "int" || typeName === "double") {
                    let min = item[sub + "Min"];
                    let max = item[sub + "Max"];

                    obj["min"] = (typeof min === "undefined") ? 0 : min
                    obj["max"] = (typeof max === "undefined") ? 100000 : max
                }

                lm.append({
                              "name": p,
                              "type": typeName,
                              "itemValue": item[p],
                              "choices": choices,
                              "item": item,
                              "map": obj
                          })
            }
        }

        this.item = item

        open()
    }

    ListModel {
        id: lm
        dynamicRoles: true
    }

    function displayText(text) {
        const result = text.substring(5).replace(/([A-Z])/g, " $1")
        return result.charAt(0).toUpperCase() + result.slice(1)
    }

    function getValues() {
        for (var i = 0; i < listView.count; ++i) {
            let idx = lm.get(i)
            item[idx.name] = idx.itemValue
        }
    }

    ListView {
        anchors {
            top: parent.top
            bottom: buttonBox.bottom
            left: parent.left
            right: parent.right

            margins: 8
        }

        spacing: 8

        id: listView

        clip: true

        boundsBehavior: Flickable.StopAtBounds

        model: lm

        component FieldLabel: Label {
            Layout.fillWidth: true

            text: displayText(model.name)
            font.pixelSize: 15
            color: Constants.palette.text
        }

        delegate: DelegateChooser {
            role: "type"

            // TODO: may want to find a way to avoid all this repeat code
            DelegateChoice {
                roleValue: "QString"

                RowLayout {
                    clip: true
                    width: parent.width

                    uniformCellSizes: true

                    FieldLabel {}

                    TextField {
                        id: txt
                        Layout.fillWidth: true

                        font.pixelSize: 15
                        text: model.itemValue

                        onTextEdited: model.itemValue = text
                    }
                }
            }

            DelegateChoice {
                roleValue: "bool"

                RowLayout {
                    clip: true
                    width: parent.width

                    uniformCellSizes: true

                    FieldLabel {}

                    CheckBox {
                        checked: model.itemValue

                        onClicked: model.itemValue = checked

                        indicator.implicitHeight: 20
                        indicator.implicitWidth: 20
                    }
                }
            }

            DelegateChoice {
                roleValue: "int"

                RowLayout {
                    clip: true
                    width: parent.width

                    uniformCellSizes: true

                    FieldLabel {}

                    SpinBox {
                        from: map.min
                        to: map.max

                        id: sb

                        Layout.fillWidth: true

                        font.pixelSize: 15
                        value: model.itemValue

                        onValueModified: model.itemValue = value
                    }
                }
            }

            DelegateChoice {
                roleValue: "double"

                RowLayout {
                    clip: true
                    width: parent.width

                    uniformCellSizes: true

                    FieldLabel {}

                    DoubleSpinBox {
                        from: map.min
                        to: map.max

                        id: dsb
                        font.pixelSize: 15
                        Layout.fillWidth: true

                        value: model.itemValue

                        stepSize: 0.1

                        onValueModified: model.itemValue = value
                    }
                }
            }

            DelegateChoice {
                roleValue: "QColor"

                RowLayout {
                    clip: true
                    width: parent.width

                    uniformCellSizes: true

                    FieldLabel {}

                    TextField {
                        id: colorField
                        font.pixelSize: 15
                        Layout.fillWidth: true

                        text: model.itemValue

                        onTextEdited: model.itemValue = text
                    }

                    Button {
                        Layout.fillWidth: true
                        text: "Pick"

                        function setColor() {
                            colorField.text = colorDialog.selectedColor
                            model.itemValue = colorDialog.selectedColor
                            colorDialog.accepted.disconnect(setColor)
                        }

                        onClicked: {
                            colorDialog.selectedColor = colorField.text
                            colorDialog.accepted.connect(setColor)
                            colorDialog.open()
                        }
                    }
                }
            }

            DelegateChoice {
                roleValue: "list"

                RowLayout {
                    clip: true
                    width: parent.width

                    uniformCellSizes: true

                    FieldLabel {}

                    function setValue(v) {
                        model.itemValue = v
                    }

                    ComboBox {
                        model: choices

                        delegate: ItemDelegate {
                            id: delegate

                            width: choices.width
                            contentItem: Text {
                                text: modelData
                                color: "white"
                                font.pixelSize: 15
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            highlighted: choices.highlightedIndex === index
                        }

                        id: cb
                        font.pixelSize: 15
                        Layout.fillWidth: true

                        Component.onCompleted: {
                            currentIndex = choices.indexOf(itemValue)
                            currentIndexChanged.connect(updateValue)
                        }

                        function updateValue() {
                            setValue(valueAt(currentIndex))
                        }
                    }
                }
            }

            DelegateChoice {
                roleValue: "QVariantList"

                GridLayout {
                    clip: true
                    width: parent.width

                    rowSpacing: 0

                    rows: 4
                    columns: 2

                    FieldLabel {}

                    function setValue(v) {
                        model.itemValue = v
                    }

                    function getValue() {
                        // stupid workaround because of cancer
                        // itemValue gets corrupted to a QQmlListModel for some reason
                        // kill me
                        return model.item[model.name]
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

                        Component.onCompleted: {
                            let vl = map.valueName
                            let iv = getValue()

                            for (let i = 0; i < iv.length; ++i) {
                                tblModel.add(iv[i]["Value"], iv[i][vl])
                                tbl.resetHeight()
                            }

                            setValue(tblModel.asList())
                            tblModel.dataChanged.connect(() => setValue(tblModel.asList()))
                        }

                        model: MapModel {
                            id: tblModel
                            valueName: map.valueName
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

                    Button {
                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        Layout.row: 1
                        Layout.column: 0
                        text: "Add"

                        onClicked: {
                            tblModel.add("", "")
                            setValue(tblModel.asList())
                            tbl.resetHeight()
                        }
                    }

                    Button {
                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        Layout.row: 2
                        Layout.column: 0
                        text: "Delete"

                        onClicked: {
                            tblModel.remove(tbl.currentRow)
                            setValue(tblModel.asList())
                            tbl.resetHeight()
                        }
                    }
                }
            }

            DelegateChoice {
                roleValue: "QSizeF"

                RowLayout {
                    clip: true
                    width: parent.width


                    FieldLabel {
                        Layout.rowSpan: 4
                    }

                    Label {
                        text: "Width:"
                        font.pixelSize: 15
                    }

                    SpinBox {
                        from: 0
                        to: 2000

                        id: width

                        Layout.fillWidth: true

                        font.pixelSize: 15
                        value: model.itemValue.width

                        onValueModified: model.itemValue.width = value
                    }

                    Label {
                        text: "Height:"
                        font.pixelSize: 15
                    }

                    SpinBox {
                        from: 0
                        to: 2000

                        id: height

                        Layout.fillWidth: true

                        font.pixelSize: 15
                        value: model.itemValue.height

                        onValueModified: model.itemValue.height = value
                    }
                }
            }
        }
    }

    DialogButtonBox {
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            margins: 8
        }

        id: buttonBox
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

        onAccepted: {
            getValues()
            widgetConf.accept()
        }

        onRejected: {
            widgetConf.reject()
        }
    }
}
