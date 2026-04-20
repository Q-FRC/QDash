// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 6.4
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic as B
import QtQuick.Layouts 2.15

import Carboxyl.Clover
import QDash.Items
import QDash.Config

import QDash.Native.Helpers

Rectangle {
    signal moved(real x, real y)

    property string item_topic

    id: widget

    // *all* widgets must define their properties in this list.
    // They are then updated, and passed to the JSON.
    // This is primarily done to reduce startup overhead on AMD E-350
    property list<string> propertyKeys

    // default to disconnected and invalid
    property bool connected: false
    property bool valid: false

    property bool tvOverlap: false

    z: 3

    radius: 12
    property int titleFontSize: QDashSettings.defaultTitleFontSize

    property alias dragArea: dragArea
    property alias titleField: titleField

    // right-click menu stuff, incl. extensions
    property alias rcMenu: rcMenuLoader.item
    property Component menuExtension: null
    property var _extensionInstance: null

    property int minWidth: grid.colWidth - 16
    property int minHeight: grid.rowHeight - 16
    property bool dragging: Drag.active || dragForced

    color: Clover.theme.dark

    Drag.active: dragArea.drag.active

    Connections {
        target: tab
        function onTopicViewRectChanged() {
            let rc = tab.topicViewRect
            let tvRight = rc.x + rc.width
            widget.tvOverlap = tvRight >= x
        }
    }

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
        resizeActive = false
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

    function dragTapped() {
        if (dragForced || widget.Drag.active) {
            cancelDrag()
        } else {
            startDrag()
        }
    }

    TapHandler {
        gesturePolicy: TapHandler.ReleaseWithinBounds

        onSingleTapped: dragTapped()
        onDoubleTapped: openContextMenu()
        onLongPressed: openContextMenu()

        longPressThreshold: 250
        enabled: !widget.tvOverlap
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

        propertyKeys.push("titleFontSize")

        // TODO(crueter): Properties should have some way to define a default.
        // This way if a property is at default and it changes in an update or a setting, it will change in
        // dependent widgets.
        for (var i = 0; i < propertyKeys.length; i++) {
            let p = propertyKeys[i]
            let jsonProp = model.properties[p]

            if (typeof jsonProp !== "undefined")
                this[p] = jsonProp
        }
    }

    Connections {
        target: tab.twm
        function onBeforeSave() {
            let props = {}
            for (var i = 0; i < propertyKeys.length; i++) {
                let p = propertyKeys[i]
                props[p] = widget[p]
            }
            model.properties = props
        }
    }

    width: grid.colWidth * model.colSpan - 16
    height: grid.rowHeight * model.rowSpan - 16

    x: grid.colWidth * model.column + 8
    y: grid.rowHeight * model.row + 8

    Connections {
        target: tab

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

    /** RIGHT-CLICK MENU **/
    Loader {
        id: rcMenuLoader
        active: false
        asynchronous: true
        sourceComponent: Component {
            Menu {
                Component.onCompleted: {
                    if (!_extensionInstance && menuExtension)
                        _extensionInstance = menuExtension.createObject(widget)

                    if (_extensionInstance) {
                        if (_extensionInstance instanceof Menu)
                            addMenu(_extensionInstance)
                        // TODO(crueter): More extensibility?
                        else
                            addItem(_extensionInstance)
                    }

                    // RIP iOS
                    if (CompileDefinitions.brokenMenus) {
                        open()
                    } else {
                        popup()
                    }
                }

                MenuItem {
                    text: "Delete"
                    onTriggered: twm.remove(model.idx)
                }

                MenuItem {
                    text: "Configure"
                    onTriggered: {
                        if (configLoader) {
                            configLoader.active = true
                        } else if (config) {
                            config.open()
                        }
                    }
                }

                MenuItem {
                    text: "Copy"
                    onTriggered: copy(idx)
                }

                onClosed: rcMenuLoader.active = false
            }
        }
    }

    function openContextMenu() {
        if (!rcMenuLoader.active)
            rcMenuLoader.active = true
    }

    MouseArea {
        id: dragArea

        anchors.fill: parent
        drag.target: parent

        acceptedButtons: Qt.AllButtons
        pressAndHoldInterval: 100
        hoverEnabled: true

        onPressed: mouse => {
                       focus = true
                       if (mouse.button === Qt.RightButton) {
                           drag.target = null
                           widget.openContextMenu()
                       } else if (mouse.button === Qt.LeftButton) {
                           dragTapped()
                       }
                   }

        onReleased: mouse => {
                        if (mouse.button === Qt.LeftButton) {
                            if (CompileDefinitions.tapOnly)
                            mouse.accepted = false

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

        onDoubleClicked: openContextMenu()
    }

    /* RESIZE HANDLING */
    HoverHandler {
        id: hoverHandler
    }

    Loader {
        active: CompileDefinitions.tapOnly
                || (hoverHandler.hovered && !widget.tvOverlap
                    && !(widget.dragForced || widget.Drag.active))

        anchors.fill: parent

        z: 26

        sourceComponent: Component {
            ResizeComponent {}
        }
    }

    /* ACTUAL DATA */
    B.TextField {
        id: titleField
        z: enabled ? 25 : 0

        font.pixelSize: titleFontSize
        font.bold: true

        text: model.title
        color: Clover.theme.text

        enabled: !(Drag.active || dragForced) && !widget.tvOverlap

        onTextEdited: model.title = text

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        background: Rectangle {
            color: Clover.theme.currentAccent
        }

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }


    /**
    * This is the "base" configuration dialog containing the NT and font settings.
    * Copy it for your widget.
    */
    Loader {
        // Uncomment this for your widget
        // id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: Component {
            BaseConfigDialog {
                content: ColumnLayout {
                    id: layout
                    spacing: 12
                    anchors.fill: parent
                    anchors.leftMargin: 2

                    SectionHeader {
                        label: "Font Settings"
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        label: "Title Font Size"

                        bindedProperty: "titleFontSize"
                    }

                    SectionHeader {
                        label: "NT Settings"
                    }

                    LabeledTextField {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        label: "Topic"

                        bindedProperty: "item_topic"
                    }
                }
            }
        }
    }
}
