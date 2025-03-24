import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs

import QFRCDashboard

Row {
    id: tv
    z: 5

    property alias menuAnim: menuAnim

    property string closedText: ">>"
    property string openText: "<<"

    width: (parent.width / 3) + 40 * Constants.scalar
    height: parent.height

    SmoothedAnimation {
        id: menuAnim
        target: tv
        property: "anchors.leftMargin"
        duration: 500
    }

    signal open
    signal close

    signal addWidget(string name, string topic, string type)
    signal dragging(point pos)
    signal dropped(point pos)

    function widgetAdd(name, topic, type) {
        button.text = closedText
        close()
        addWidget(name, topic, type)
    }

    Rectangle {
        id: topicView
        radius: 10 * Constants.scalar

        width: parent.width - 40 * Constants.scalar
        height: parent.height

        color: Constants.palette.menu
        border {
            color: Constants.palette.menuBorder
            width: 3
        }

        BetterTextField {
            id: search

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right

                margins: 10 * Constants.scalar
                topMargin: 18 * Constants.scalar
            }

            font.pixelSize: 16 * Constants.scalar

            placeholderText: "Search"

            onTextEdited: {
                topicsSorted.setFilterWildcard("*" + text + "*")
            }
        }

        // modified from Qt's example TreeView
        TreeView {
            id: treeView
            anchors {
                top: search.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                margins: 10
            }

            clip: true

            boundsBehavior: Flickable.StopAtBounds

            selectionModel: ItemSelectionModel {}

            model: topicsSorted

            delegate: Item {
                DragHandler {
                    id: dh
                    target: null

                    property bool ready: false

                    onActiveChanged: if (!active && ready) {
                                         ready = false
                                         dropped(centroid.position)
                                     }

                    function drag() {
                        let global = mapToItem(topicView, centroid.position)
                        if (!topicView.contains(global)) {
                            if (!ready) {
                                widgetAdd(model.name, model.topic, model.type)

                                ready = true
                            }

                            let p = mapToItem(tv, centroid.position)
                            p.x += tv.x
                            dragging(p)
                        }
                    }

                    yAxis.onActiveValueChanged: {
                        drag()
                    }

                    xAxis.onActiveValueChanged: {
                        drag()
                    }
                }

                // implicitWidth: padding + label.x + label.implicitWidth + padding
                implicitHeight: label.implicitHeight * 1.5

                implicitWidth: topicView.width - 20

                readonly property real indentation: 20 * Constants.scalar
                readonly property real padding: 5 * Constants.scalar

                // Assigned to by TreeView:
                required property TreeView treeView
                required property bool isTreeNode
                required property bool expanded
                required property int hasChildren
                required property int depth
                required property int row
                required property int column
                required property bool current

                // Rotate indicator when expanded by the user
                // (requires TreeView to have a selectionModel)
                property Animation indicatorAnimation: NumberAnimation {
                    target: indicator
                    property: "rotation"
                    from: expanded ? 0 : 90
                    to: expanded ? 90 : 0
                    duration: 100
                    easing.type: Easing.OutQuart
                }
                TableView.onPooled: indicatorAnimation.complete()
                TableView.onReused: if (current)
                                        indicatorAnimation.start()
                onExpandedChanged: indicator.rotation = expanded ? 90 : 0

                Rectangle {
                    id: background
                    anchors.fill: parent
                    color: row === treeView.currentRow ? palette.highlight : Constants.palette.menuItem
                    opacity: (treeView.alternatingRows
                              && row % 2 !== 0) ? 0.3 : 0.1
                }

                Label {
                    id: indicator
                    x: padding + (depth * indentation)
                    anchors.verticalCenter: parent.verticalCenter
                    visible: isTreeNode && hasChildren
                    text: "▶"
                    color: Constants.palette.text

                    TapHandler {
                        onSingleTapped: {
                            let index = treeView.index(row, column)
                            treeView.selectionModel.setCurrentIndex(
                                        index, ItemSelectionModel.NoUpdate)
                            treeView.toggleExpanded(row)
                        }
                    }

                    font.pixelSize: 17 * Constants.scalar
                }

                Label {
                    id: label
                    x: padding + (isTreeNode ? (depth + 1) * indentation : 0)
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - padding - x - typeLabel.width
                    clip: true
                    text: model.name

                    color: Constants.palette.text

                    font.pixelSize: 17 * Constants.scalar
                }

                Label {
                    id: typeLabel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    clip: true
                    text: model.type

                    color: Constants.palette.text

                    font.pixelSize: 17 * Constants.scalar
                }
            }
        }
    }

    Button {
        id: button
        text: closedText

        width: 40 * Constants.scalar
        height: 40 * Constants.scalar

        font.pixelSize: 18 * Constants.scalar

        onClicked: {
            if (text === closedText) {
                open()
                text = openText
            } else {
                close()
                text = closedText
            }
        }

        contentItem: Text {
            font: parent.font

            text: parent.text
            color: Constants.palette.text

            width: 40 * Constants.scalar
            height: 40 * Constants.scalar

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            anchors.fill: parent
            color: Constants.palette.dialogBg
        }
    }
}
