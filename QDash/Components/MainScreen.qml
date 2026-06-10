// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import Carboxyl.Contour
import QDash.Dialogs
import QtCore
import QtQuick 6.8
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

Rectangle {
    id: mainScreen

    property var clipboard: null
    property bool readyDragging

    function addWidget(title, topic, type) {
        currentTab().fakeAdd(title, topic, type)
    }

    function closeTab() {
        if (!currentTab())
            return
        tabClose.active = true
    }

    function configTab() {
        let tab = currentTab()
        if (!tab)
            return
        tabConfigDialog.rows = tab.rows
        tabConfigDialog.columns = tab.cols
        tabConfigDialog.name = tab.name()
        tabConfigDialog.active = true
    }

    function currentTab() {
        return swipe.currentItem
    }

    // This function is called for copying and dragging from the TopicView
    // In-widget drags are done via the Drag object
    function drag(pos, fromList) {
        if (currentTab() !== null) {
            let w = currentTab().latestWidget

            w.x = pos.x
            w.y = pos.y - (fromList ? tabs.height + 5 : 0) - w.titleField.height

            w.width = currentTab().colWidth - 16
            w.height = currentTab().rowWidth - 16

            w.mrowSpan = 1
            w.mcolumnSpan = 1

            if (!readyDragging) {
                readyDragging = true
                w.dragForced = true

                mainScreen.z = 3
                w.startDrag()
            }
        }
    }

    function drop(pos, fromList) {
        if (currentTab() !== null) {
            readyDragging = false
            let w = currentTab().latestWidget
            if (typeof w === 'undefined' || w === null)
                return
            if (!currentTab().lastOpSuccessful) {
                w.cancelDrag()
                currentTab().removeLatest()
            } else {
                let point = w.getPoint()

                w.mrow = point.y
                w.mcolumn = point.x

                w.z = 3
                w.visible = true
                w.cancelDrag()

                w.fixSize()
                TabListModel.modified = true
            }
        }
    }

    /** TAB SETTINGS */
    function newTab() {
        tabNameDialog.active = true
    }

    /** PASTE */
    function paste() {
        if (clipboard != null) {
            currentTab().paste(clipboard)
        }
    }

    function setTab(value) {
        let idx = TabListModel.tabNamed(value)
        if (idx !== -1) {
            swipe.setCurrentIndex(idx)
        }
    }

    color: Clover.theme.base

    Component.onCompleted: {
        TopicStore.subscribe("/QDash/Tab", setTab)
    }

    Shortcut {
        sequences: ["Ctrl+Tab"]

        onActivated: if (swipe.currentIndex < swipe.count - 1) {
            swipe.incrementCurrentIndex()
        } else {
            swipe.setCurrentIndex(0)
        }
    }

    Shortcut {
        sequences: ["Ctrl+Shift+Tab"]

        onActivated: if (swipe.currentIndex > 0) {
            swipe.decrementCurrentIndex()
        } else {
            swipe.setCurrentIndex(swipe.count - 1)
        }
    }

    TopicView {
        id: tv

        z: 25

        onAddWidget: (title, topic, type) => {
            currentTab().add(title, topic, type)
        }
        onClose: {
            menuAnim.to = -(parent.width / 3)
            menuAnim.from = 0
            menuAnim.start()
        }
        onDragging: pos => drag(pos, true)
        onDropped: pos => drop(pos, true)
        onOpen: {
            menuAnim.from = -(parent.width / 3)
            menuAnim.to = 0
            menuAnim.start()
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            leftMargin: -(parent.width / 3)
            top: parent.top
        }
    }

    TabNameDialog {
        id: tabNameDialog

        onAccepted: {
            TabListModel.add(text)
            swipe.setCurrentIndex(swipe.count - 1)
            TabListModel.modified = true
        }
    }

    TabDialog {
        id: tabConfigDialog

        onAccepted: {
            let tab = mainScreen.currentTab()
            if (!tab)
                return
            tab.setSize(rows, columns)
            tab.setName(name)

            TabListModel.modified = true
        }
    }

    /** CLOSE TAB */
    TabCloseDialog {
        id: tabClose

        onAccepted: {
            TabListModel.modified = true
            TabListModel.remove(swipe.currentIndex)
        }
    }

    /** CONTENT */
    Label {
        anchors.centerIn: parent
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        text: "Welcome to QDash!\n" + //
        "To get started, connect to your robot WiFi\n" + //
        "and go to Settings (Ctrl+Comma).\nAdd a tab with Ctrl+T, and add a widget\n" + //
        "through the arrow menu on the left."
        visible: swipe.count === 0
        z: 0
    }

    SwipeView {
        id: swipe

        currentIndex: tabs.currentIndex
        z: 0

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            top: tabs.bottom
        }

        Repeater {
            id: swRep

            model: TabListModel

            Tab {
                id: tab

                enabled: SwipeView.isCurrentItem
                height: swipe.height
                topicViewRect: mapFromItem(mainScreen, tv.geometry)
                width: swipe.width

                onCopying: pos => drag(pos, false)
                onDropped: pos => drop(pos, false)
                onStoreWidget: w => clipboard = w
            }
        }
    }

    CarboxylTabBar {
        id: tabs

        contentHeight: 40
        currentIndex: swipe.currentIndex
        font.pixelSize: 16
        height: 40
        position: TabBar.Header
        spacing: 2

        anchors {
            left: tv.right
            leftMargin: 0
            right: parent.right
            rightMargin: 0
            top: parent.top
        }

        Repeater {
            id: tabRep

            model: TabListModel

            CarboxylTabButton {
                text: model.title
                width: Math.max(100, tabs.width / 6)
            }
        }
    }
}
