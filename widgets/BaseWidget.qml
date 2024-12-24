import QtQuick 6.7
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QFRCDashboard 1.0
import QFDFlags 1.0

Rectangle {
    id: rect
    width: 100
    height: 100
    z: 3

    radius: 12

    property int item_titleFontSize: 20

    property alias titleField: titleField
    property alias rcMenu: rcMenu
    property alias dragArea: dragArea

    color: Constants.palette.widgetBg

    Drag.active: dragArea.drag.active

    signal resizeBegin(var drag)
    signal resizeEnd(var drag)
    signal dragEnd(var drag)
    signal cancelDrag

    onResizeBegin: drag => {
                       drag.source = this
                       rep.beginResize(drag)
                   }

    onResizeEnd: drag => {
                     drag.source = this
                     rep.endResize(drag)
                 }

    onDragEnd: drag => {
                   let source = Qt.rect(drag.source.mcolumn, drag.source.mrow,
                                        drag.source.mcolumnSpan,
                                        drag.source.mrowSpan)
                   rep.intersectionCheck(drag, source)
               }

    onCancelDrag: rep.fullCancelDrag()

    property point beginDrag
    property rect beginResize

    property bool caught: false

    property int mrow
    property int mcolumn
    property int mrowSpan
    property int mcolumnSpan

    onMrowChanged: model.row = mrow
    onMcolumnChanged: model.column = mcolumn
    onMrowSpanChanged: model.rowSpan = mrowSpan
    onMcolumnSpanChanged: model.colSpan = mcolumnSpan

    function remove() {
        twm.remove(model.row, model.column)
    }

    Component.onCompleted: {
        tab.latestWidget = this

        mrow = model.row
        mcolumn = model.column
        mrowSpan = model.rowSpan
        mcolumnSpan = model.colSpan

        for (var p in this) {
            if (p.startsWith("item_") && typeof this[p] !== "function") {
                let propName = p
                let substr = propName.substring(5)
                let prop = model.properties[substr]
                this[p] = typeof prop === "undefined" ? this[propName] : prop

                if (substr === "topic") {
                    this.item_topicChanged.connect(() => {
                                                       model.topic = this.item_topic
                                                   })
                } else {
                    this[p + "Changed"].connect(() => {
                                                    let x = model.properties
                                                    x[substr] = this[propName]
                                                    model.properties = x
                                                })
                }
            }
        }
    }

    Menu {
        id: rcMenu

        MenuItem {
            text: "Delete"
            onTriggered: twm.remove(model.idx)
        }

        MenuItem {
            text: "Configure"
            onTriggered: config.openDialog()
        }

        MenuItem {
            text: "Copy"
            onTriggered: copy(idx)
        }
    }

    MouseArea {
        id: dragArea
        z: 0

        acceptedButtons: Qt.AllButtons

        anchors {
            fill: parent

            margins: 6
        }

        drag.target: parent

        pressAndHoldInterval: 100

        onPressed: mouse => {
                       if (mouse.button === Qt.RightButton) {
                           drag.target = null
                           rcMenu.popup()
                       } else if (mouse.button === Qt.LeftButton) {
                           drag.target = parent

                           rect.beginDrag = Qt.point(parent.x, parent.y)
                           rect.Drag.hotSpot = Qt.point(mouse.x, mouse.y)
                           parent.z = 4

                           resizeBegin(rect.Drag)
                       }
                   }

        onReleased: {
            if (drag.target === null) {
                Layout.rowSpan = grid.rows + 1
                Layout.columnSpan = grid.columns + 1
                Layout.row = grid.rows + 1
                Layout.column = grid.columns + 1
                update()

                Layout.rowSpan = mrowSpan
                Layout.columnSpan = mcolumnSpan
                Layout.column = mcolumn
                Layout.row = mrow
            } else if (!rect.caught) {
                backAnimX.from = rect.x
                backAnimX.to = beginDrag.x
                backAnimY.from = rect.y
                backAnimY.to = beginDrag.y
                backAnim.start()

                cancelDrag()
            } else {
                rect.Drag.drop()

                // dragEnd(rect.Drag)
                parent.z = 3
                cancelDrag()
            }
        }

        ParallelAnimation {
            id: backAnim
            SmoothedAnimation {
                id: backAnimX
                target: rect
                property: "x"
                duration: 250
            }
            SmoothedAnimation {
                id: backAnimY
                target: rect
                property: "y"
                duration: 250
            }

            onFinished: {
                x = rect.beginDrag.x
                y = rect.beginDrag.y
            }
        }
    }

    ParallelAnimation {
        id: resizeBackAnim
        SmoothedAnimation {
            id: resizeBackAnimX
            target: rect
            property: "x"
            duration: 250
        }
        SmoothedAnimation {
            id: resizeBackAnimY
            target: rect
            property: "y"
            duration: 250
        }
        SmoothedAnimation {
            id: resizeBackAnimWidth
            target: rect
            property: "width"
            duration: 250
        }
        SmoothedAnimation {
            id: resizeBackAnimHeight
            target: rect
            property: "height"
            duration: 250
        }

        onFinished: {
            width = rect.beginResize.width
            height = rect.beginResize.height
            x = rect.beginResize.x
            y = rect.beginResize.y
        }
    }

    function resetResize(mouse) {
        rect.beginDrag = Qt.point(rect.x, rect.y)
        rect.beginResize = Qt.rect(rect.x, rect.y, rect.width, rect.height)
        rect.Drag.hotSpot = Qt.point(mouse.x, mouse.y)
        rect.z = 4

        resizeBegin(rect.Drag)
    }

    function releaseResize() {
        if (!rect.caught) {
            resizeBackAnimX.from = rect.x
            resizeBackAnimX.to = beginResize.x
            resizeBackAnimY.from = rect.y
            resizeBackAnimY.to = beginResize.y
            resizeBackAnimWidth.from = rect.width
            resizeBackAnimWidth.to = beginResize.width
            resizeBackAnimHeight.from = rect.height
            resizeBackAnimHeight.to = beginResize.height
            resizeBackAnim.start()

            cancelDrag()
        } else {
            rect.Drag.drop()

            resizeEnd(rect.Drag)

            rect.z = 3
        }
    }

    /* RESIZE ANCHORS */
    Repeater {
        model: [QFDFlags.RIGHT, QFDFlags.LEFT, QFDFlags.TOP, QFDFlags.BOTTOM, QFDFlags.RIGHT
            | QFDFlags.TOP, QFDFlags.RIGHT | QFDFlags.BOTTOM, QFDFlags.LEFT
            | QFDFlags.TOP, QFDFlags.LEFT | QFDFlags.BOTTOM]

        ResizeAnchor {
            required property int modelData
            direction: modelData
        }
    }

    /* ACTUAL DATA */
    TextField {
        id: titleField
        font.pixelSize: item_titleFontSize
        font.bold: true

        text: model.title
        color: "#DDDDDD"

        onTextEdited: model.title = text

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        background: Rectangle {
            topLeftRadius: 12
            topRightRadius: 12
            color: Constants.accent
        }

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    /**
    * This is the "base" configuration dialog containing the NT and font settings.
    * Copy it for your widget.
    */
    BaseConfigDialog {
        // id: config

        height: 450

        function openDialog() {
            topicField.open()
            titleFontField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
        }

        ColumnLayout {
            id: layout
            spacing: 25

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: parent.headerHeight + 12
                bottomMargin: 45

                leftMargin: 5
                rightMargin: 5
            }

            SectionHeader {
                label: "Font Settings"
            }

            LabeledSpinBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: titleFontField

                label: "Title Font Size"

                bindedProperty: "item_titleFontSize"
                bindTarget: rect
            }

            SectionHeader {
                label: "NT Settings"
            }

            LabeledTextField {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: topicField

                label: "Topic"

                bindedProperty: "item_topic"
                bindTarget: rect
            }
        }
    }
}
