// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick 6.4
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4
import QtQuick.Dialogs

import Carboxyl.Clover
import QDash.Dialogs

import Carboxyl.Contour

Rectangle {
    id: mainScreen
    color: Clover.theme.base

    property bool readyDragging
    property var clipboard: null

    Shortcut {
        sequences: ["Ctrl+Tab"]
        onActivated: swipe.incrementCurrentIndex()
    }

    Shortcut {
        sequences: ["Ctrl+Shift+Tab"]
        onActivated: swipe.decrementCurrentIndex()
    }

    function setTab() {
        swipe.setCurrentIndex(tlm.selectedTab)
    }

    Component.onCompleted: {
        tlm.onSelectedTabChanged.connect(setTab)
    }

    function drag(pos, fromList) {
        if (currentTab() !== null) {
            let w = currentTab().latestWidget
            w.x = pos.x
            w.y = pos.y - (fromList ? tabs.height + 5 : 0)

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
                if (fromList)
                    currentTab().removeLatest()
            } else {
                let point = w.getPoint()

                w.mrow = point.y
                w.mcolumn = point.x

                w.z = 3
                w.visible = true
                w.cancelDrag()

                w.fixSize()
            }
        }
    }

    TopicView {
        id: tv

        z: 25

        onAddWidget: (title, topic, type) => {
                         currentTab().add(title, topic, type)
                     }

        anchors {
            left: parent.left
            leftMargin: -(parent.width / 3)

            top: parent.top
            bottom: parent.bottom
        }

        onOpen: {
            menuAnim.from = -(parent.width / 3)
            menuAnim.to = 0
            menuAnim.start()
        }

        onClose: {
            menuAnim.to = -(parent.width / 3)
            menuAnim.from = 0
            menuAnim.start()
        }

        onDragging: pos => drag(pos, true)

        onDropped: pos => drop(pos, true)
    }

    TabNameDialog {
        id: tabNameDialog
        onAccepted: addTab()
    }

    TabDialog {
        id: tabConfigDialog
        onAccepted: setTabConfig()
    }

    /** TAB SETTINGS */
    function addTab() {
        tlm.add(tabNameDialog.text)
        swipe.setCurrentIndex(swipe.count - 1)
    }

    function newTab() {
        tabNameDialog.open()
    }

    function setTabConfig() {
        currentTab().setSize(tabConfigDialog.rows, tabConfigDialog.columns)
        currentTab().setName(tabConfigDialog.name)
    }

    function configTab() {
        tabConfigDialog.openUp(currentTab().rows, currentTab().cols,
                               currentTab().name())
    }

    function currentTab() {
        return swipe.currentItem
    }

    /** CLOSE TAB */
    TabCloseDialog {
        id: tabClose

        onAccepted: tlm.remove(swipe.currentIndex)
    }

    function closeTab() {
        tabClose.open()
    }

    /** PASTE */
    function paste() {
        if (clipboard != null) {
            currentTab().paste(clipboard)
        }
    }

    function addWidget(title, topic, type) {
        currentTab().fakeAdd(title, topic, type)
    }

    /** CONTENT */
    Text {
        color: Clover.theme.text
        font.pixelSize: 20

        horizontalAlignment: Text.AlignHCenter

        text: "Welcome to QDash!\n" + "To get started, connect to your robot WiFi\n"
              + "and go to QDashSettings (Ctrl+Comma).\n"
              + "Add a tab with Ctrl+T, and add a widget\n" + "through the arrow menu on the left."

        anchors.centerIn: parent
        z: 0
    }

    SwipeView {
        id: swipe

        z: 0

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            topMargin: 40
        }

        currentIndex: tabs.currentIndex
        Repeater {
            id: swRep
            model: tlm

            Tab {
                width: swipe.width
                height: swipe.height

                onCopying: pos => drag(pos, false)

                onDropped: pos => drop(pos, false)

                onStoreWidget: w => clipboard = w
            }
        }
    }

    CarboxylTabBar {
        id: tabs
        height: 45
        contentHeight: 40

        anchors {
            top: parent.top
            left: tv.right
            right: parent.right

            leftMargin: 0
            rightMargin: 0
        }

        position: TabBar.Header
        currentIndex: swipe.currentIndex
        spacing: 2

        Repeater {
            id: tabRep
            model: tlm

            CarboxylTabButton {
                text: model.title

                width: Math.max(100, tabs.width / 6)
                height: 40
            }
        }
    }
}
