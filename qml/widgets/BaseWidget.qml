import QtQuick 6.7
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import QFRCDashboard 1.0

Rectangle {
    signal moved(real x, real y)

    property string item_topic

    id: widget
    clip: true

    // default to disconnected and invalid
    property bool connected: false
    property bool valid: false

    z: 3

    border {
        color: "transparent"
        width: 10 * Constants.scalar
    }

    radius: 12 * Constants.scalar
    property int item_titleFontSize: 16

    property alias dragArea: dragArea
    property alias titleField: titleField
    property alias rcMenu: rcMenu

    property int minWidth: grid.colWidth - 16
    property int minHeight: grid.rowHeight - 16

    color: Constants.palette.widgetBg

    Drag.active: dragArea.drag.active

    function checkDrag() {
        if (Drag.active || dragForced) {
            // only call this to get the green/red rectangle outline
            grid.validSpot(x, y, row, column, rowSpan, colSpan, !dragForced)
        }
    }

    function checkResize() {
        if (width < grid.colWidth - 16) {
            width = grid.colWidth - 16
        }

        if (height < grid.rowHeight - 16) {
            height = grid.rowHeight - 16
        }

        if (resizeActive) {
            grid.validResize(width, height, x, y, row, column, rowSpan, colSpan)
        }
    }

    function cancelDrag() {
        dragForced = false
        Drag.cancel()
        grid.resetValid()
    }

    function startDrag() {
        originalRect = Qt.rect(widget.x, widget.y, widget.width, widget.height)
        dragArea.drag.target = widget
        widget.z = 4
    }

    function startResize() {
        originalRect = Qt.rect(widget.x, widget.y, widget.width, widget.height)
        widget.resizeActive = true
        widget.z = 4
    }

    function getPoint() {
        return grid.getPoint(x, y, false)
    }

    // Drag/Resize animations
    ParallelAnimation {
        id: resizeBackAnim
        SmoothedAnimation {
            id: resizeBackAnimX
            target: widget
            property: "x"
            duration: 250
        }
        SmoothedAnimation {
            id: resizeBackAnimY
            target: widget
            property: "y"
            duration: 250
        }
        SmoothedAnimation {
            id: resizeBackAnimWidth
            target: widget
            property: "width"
            duration: 250
        }
        SmoothedAnimation {
            id: resizeBackAnimHeight
            target: widget
            property: "height"
            duration: 250
        }

        onFinished: {
            width = widget.originalRect.width
            height = widget.originalRect.height
            x = widget.originalRect.x
            y = widget.originalRect.y
        }
    }

    function animateBacksize() {
        resizeBackAnimX.from = widget.x
        resizeBackAnimX.to = originalRect.x
        resizeBackAnimY.from = widget.y
        resizeBackAnimY.to = originalRect.y
        resizeBackAnimWidth.from = widget.width
        resizeBackAnimWidth.to = originalRect.width
        resizeBackAnimHeight.from = widget.height
        resizeBackAnimHeight.to = originalRect.height
        resizeBackAnim.start()
    }

    onXChanged: checkDrag()
    onYChanged: checkDrag()

    onWidthChanged: checkResize()
    onHeightChanged: checkResize()

    property int mrow
    property int mcolumn
    property int mrowSpan
    property int mcolumnSpan

    property rect originalRect: Qt.rect(0, 0, 0, 0)

    property bool resizeActive: false
    property bool dragForced: false

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

        fixSize()

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
                                                    logs.debug(
                                                        "Widget",
                                                        "Updating property " + propName
                                                        + " for widget " + model.title
                                                        + " to " + this[propName])

                                                    let x = model.properties
                                                    x[substr] = this[propName]
                                                    model.properties = x
                                                })
                }
            }
        }
    }

    width: grid.colWidth * model.colSpan - 16
    height: grid.rowHeight * model.rowSpan - 16

    x: grid.colWidth * model.column + 8
    y: grid.rowHeight * model.row + 8

    Connections {
        target: grid

        function onColWidthChanged() {
            widget.fixSize()
        }

        function onRowHeightChanged() {
            widget.fixSize()
        }
    }

    function fixSize() {
        width = grid.colWidth * model.colSpan - 16
        height = grid.rowHeight * model.rowSpan - 16
        x = grid.colWidth * model.column + 8
        y = grid.rowHeight * model.row + 8
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

        anchors.fill: parent

        drag.target: parent
        acceptedButtons: Qt.AllButtons
        pressAndHoldInterval: 100

        onPressed: mouse => {
                       focus = true
                       if (mouse.button === Qt.RightButton) {
                           drag.target = null
                           rcMenu.popup()
                       } else if (mouse.button === Qt.LeftButton) {
                           startDrag()
                       }
                   }

        onReleased: mouse => {
                        if (mouse.button === Qt.LeftButton) {
                            drag.target = null

                            if (grid.validSpot(widget.x, widget.y, row, column,
                                               rowSpan, colSpan, true)) {

                                let newPoint = grid.getPoint(widget.x,
                                                             widget.y, true)

                                model.row = newPoint.y
                                model.column = newPoint.x

                                fixSize()
                            } else {
                                animateBacksize()
                            }
                            grid.resetValid()

                            widget.z = 3
                        }
                    }
    }

    /* RESIZE ANCHORS */
    Repeater {
        model: [Qt.RightEdge, Qt.LeftEdge, Qt.TopEdge, Qt.BottomEdge, Qt.RightEdge
            | Qt.TopEdge, Qt.RightEdge | Qt.BottomEdge, Qt.LeftEdge
            | Qt.TopEdge, Qt.LeftEdge | Qt.BottomEdge]

        ResizeAnchor {

            required property int modelData
            direction: modelData

            mouseArea.onPressed: mouse => {
                                     if (mouse.button === Qt.RightButton) {
                                         drag.target = null
                                         rcMenu.popup()
                                     } else if (mouse.button === Qt.LeftButton) {
                                         startResize()
                                     }
                                 }
            mouseArea.onReleased: mouse => {
                                      if (mouse.button === Qt.LeftButton) {
                                          if (grid.validResize(widget.width,
                                                               widget.height,
                                                               widget.x,
                                                               widget.y, row,
                                                               column, rowSpan,
                                                               colSpan)) {

                                              let newSize = grid.getRect(
                                                  widget.x, widget.y,
                                                  widget.width, widget.height)

                                              model.rowSpan = newSize.height
                                              model.colSpan = newSize.width
                                              model.row = newSize.y
                                              model.column = newSize.x

                                              fixSize()
                                          } else {
                                              animateBacksize()
                                          }

                                          resizeActive = false

                                          grid.resetValid()

                                          widget.z = 3
                                      }
                                  }
        }
    }

    /* ACTUAL DATA */
    TextField {
        z: 1
        id: titleField
        font.pixelSize: item_titleFontSize * Constants.scalar
        font.bold: true

        clip: true

        text: model.title

        color: connected ? Constants.palette.text : Constants.palette.disabledText
        Behavior on color {
            ColorAnimation {
                duration: 250
            }
        }

        onTextEdited: model.title = text

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        background: Item {}

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Rectangle {
        anchors {
            top: titleField.top
            left: parent.left
            right: parent.right
            bottom: titleField.bottom
        }

        topLeftRadius: 12 * Constants.scalar
        topRightRadius: 12 * Constants.scalar
        color: Constants.accent
    }


    /**
    * This is the "base" configuration dialog containing the NT and font settings.
    * Copy it for your widget.
    */
    BaseConfigDialog {
        // Uncomment this for your widget
        // id: config
        content: ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar
            anchors.fill: parent
            anchors.leftMargin: 2
            clip: true

            SectionHeader {
                label: "Font Settings"
            }

            LabeledSpinBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: titleFontField

                label: "Title Font Size"

                bindedProperty: "item_titleFontSize"
                bindTarget: widget
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
                bindTarget: widget
            }
        }
    }
}
