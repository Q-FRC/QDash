// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Core

import QtQuick 6.8
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic as B
import QtQuick.Layouts 2.15

Rectangle {
    id: widget

    property var _extensionInstance: null

    // The content passed to BaseConfigDialog.
    property Item configContent

    // default to disconnected and invalid
    property bool connected: false
    property alias dragArea: dragArea
    property bool dragForced: false
    property bool dragging: Drag.active || dragForced
    property string item_topic
    property int mcolumn
    property int mcolumnSpan
    property Component menuExtension: null
    property int minHeight: grid.rowHeight - 16
    property int minWidth: grid.colWidth - 16
    property int mrow
    property int mrowSpan
    property rect originalRect: Qt.rect(0, 0, 0, 0)

    // *all* widgets must define their properties in this list.
    // They are then updated, and passed to the JSON.
    // This is primarily done to reduce startup overhead on AMD E-350
    property list<string> propertyKeys

    // right-click menu stuff, incl. extensions
    property alias rcMenu: rcMenuLoader.item
    property bool resizeActive: false
    property alias titleField: titleField
    property int titleFontSize: QDashSettings.defaultTitleFontSize
    property bool tvOverlap: false
    property bool valid: false

    signal moved(real x, real y)

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

    function cancelDrag() {
        dragForced = false
        resizeActive = false
        Drag.cancel()
        grid.resetValid()
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

    function dragTapped() {
        if (dragForced || widget.Drag.active) {
            cancelDrag()
        } else {
            startDrag()
        }
    }

    function fixSize() {
        width = grid.colWidth * model.colSpan - 16
        height = grid.rowHeight * model.rowSpan - 16
        x = grid.colWidth * model.column + 8
        y = grid.rowHeight * model.row + 8
    }

    function getPoint() {
        return grid.getPoint(x, y, false)
    }

    function openContextMenu() {
        if (!rcMenuLoader.active && !(Drag.active || dragForced))
            rcMenuLoader.active = true
    }

    function remove() {
        twm.remove(model.row, model.column)
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

    Drag.active: dragArea.drag.active
    color: Clover.theme.dark
    height: grid.rowHeight * model.rowSpan - 16
    radius: 12
    width: grid.colWidth * model.colSpan - 16
    x: grid.colWidth * model.column + 8
    y: grid.rowHeight * model.row + 8
    z: 3

    Component.onCompleted: {
        tab.latestWidget = this

        mrow = model.row
        mcolumn = model.column
        mrowSpan = model.rowSpan
        mcolumnSpan = model.colSpan

        fixSize()

        propertyKeys.push("titleFontSize");

        // TODO(crueter): Properties should have some way to define a default.
        // This way if a property is at default and it changes in an update or a setting, it will change in
        // dependent widgets.
        for (var i = 0; i < propertyKeys.length; i++) {
            let p = propertyKeys[i]
            let jsonProp = model.properties[p]

            if (typeof jsonProp !== "undefined")
                this[p] = jsonProp

            this[p + "Changed"].connect(() => {
                let x = model.properties
                x[p] = this[p]
                model.properties = x
            })
        }
    }
    onHeightChanged: checkResize()
    onMcolumnChanged: model.column = mcolumn
    onMcolumnSpanChanged: model.colSpan = mcolumnSpan
    onMrowChanged: model.row = mrow
    onMrowSpanChanged: model.rowSpan = mrowSpan
    onWidthChanged: checkResize()
    onXChanged: checkDrag()
    onYChanged: checkDrag()

    Connections {
        function onTopicViewRectChanged() {
            let rc = tab.topicViewRect
            let tvRight = rc.x + rc.width
            widget.tvOverlap = tvRight >= x
        }

        target: tab
    }

    // Drag/Resize animations
    ParallelAnimation {
        id: resizeBackAnim

        onFinished: {
            width = widget.originalRect.width
            height = widget.originalRect.height
            x = widget.originalRect.x
            y = widget.originalRect.y
        }

        SmoothedAnimation {
            id: resizeBackAnimX

            duration: 250
            property: "x"
            target: widget
        }

        SmoothedAnimation {
            id: resizeBackAnimY

            duration: 250
            property: "y"
            target: widget
        }

        SmoothedAnimation {
            id: resizeBackAnimWidth

            duration: 250
            property: "width"
            target: widget
        }

        SmoothedAnimation {
            id: resizeBackAnimHeight

            duration: 250
            property: "height"
            target: widget
        }
    }

    TapHandler {
        enabled: !widget.tvOverlap
        gesturePolicy: TapHandler.ReleaseWithinBounds
        longPressThreshold: 250

        onDoubleTapped: openContextMenu()
        onLongPressed: openContextMenu()
        onSingleTapped: dragTapped()
    }

    Connections {
        function onColWidthChanged() {
            widget.fixSize()
        }

        function onRowHeightChanged() {
            widget.fixSize()
        }

        target: tab
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
                        else
                            // TODO(crueter): More extensibility?
                            addItem(_extensionInstance)
                    }

                    // RIP iOS
                    if (CompileDefinitions.brokenMenus) {
                        open()
                    } else {
                        popup()
                    }
                }
                onClosed: rcMenuLoader.active = false

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
            }
        }
    }

    MouseArea {
        id: dragArea

        acceptedButtons: Qt.AllButtons
        anchors.fill: parent
        drag.target: parent
        hoverEnabled: true
        pressAndHoldInterval: 100

        onDoubleClicked: openContextMenu()
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

                if (grid.validSpot(widget.x, widget.y, row, column, rowSpan, colSpan, true)) {
                    let newPoint = grid.getPoint(widget.x, widget.y, true)

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

    /* RESIZE HANDLING */
    HoverHandler {
        id: hoverHandler
    }

    Loader {
        active: CompileDefinitions.tapOnly || (hoverHandler.hovered && !widget.tvOverlap && !(widget.dragForced || widget.Drag.active))
        anchors.fill: parent
        z: 26

        sourceComponent: ResizeComponent {}
    }

    /* ACTUAL DATA */
    B.TextField {
        id: titleField

        color: Clover.theme.text
        enabled: !(Drag.active || dragForced) && !widget.tvOverlap
        font.bold: true
        font.pixelSize: titleFontSize
        horizontalAlignment: Text.AlignHCenter
        text: model.title
        verticalAlignment: Text.AlignVCenter
        z: enabled ? 25 : 0

        background: Rectangle {
            color: Clover.theme.currentAccent
        }

        onTextEdited: model.title = text

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
    }

    Loader {
        id: configLoader

        active: false
        asynchronous: true

        sourceComponent: Component {
            BaseConfigDialog {
                content: widget.configContent

                onClosed: configLoader.active = false
            }
        }

        onLoaded: item.open()
    }
}
