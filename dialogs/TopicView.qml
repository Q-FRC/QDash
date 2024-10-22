import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import QFRCDashboard

Row {
    id: tv
    z: 4

    property alias menuAnim: menuAnim

    property bool isCamera: false

    property string closedText: isCamera ? "<<" : ">>"
    property string openText: isCamera ? ">>" : "<<"

    width: (parent.width / 3) + 40
    height: parent.height

    SmoothedAnimation {
        id: menuAnim
        target: tv
        property: "anchors." + (isCamera ? "right" : "left") + "Margin"
        duration: 500
    }

    signal open
    signal close

    signal addWidget(string name, string topic, string type)
    signal addCamera(string name, string source, var urls)
    signal dragging(point pos)
    signal dropped(point pos)

    function widgetAdd(name, topic, type) {
        button2.text = closedText
        close()
        addWidget(name, topic, type)
    }

    function cameraAdd(name, source, urls) {
        button.text = (closedText)
        close()
        addCamera(name, source, urls)
    }

    Button {
        id: button
        text: closedText

        width: isCamera ? 40 : 0
        height: isCamera ? 40 : 0

        onClicked: {
            if (text === closedText) {
                open()
                text = openText
            } else {
                close()
                text = closedText
            }
        }
    }

    Rectangle {
        id: topicView
        radius: 10

        width: parent.width - 40
        height: parent.height

        color: Constants.palette.menu
        border {
            color: Constants.palette.menuBorder
            width: 3
        }

        // modified from Qt's example TreeView
        TreeView {
            id: treeView
            anchors.fill: parent
            anchors.margins: 10
            clip: true

            boundsBehavior: Flickable.StopAtBounds

            selectionModel: ItemSelectionModel {}

            model: isCamera ? cameras : topics

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
                                if (isCamera) {
                                    cameraAdd(model.name, model.source,
                                              model.urls)
                                } else {
                                    widgetAdd(model.name, model.topic,
                                              model.type)
                                }

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

                readonly property real indentation: 20
                readonly property real padding: 5

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

                    font.pixelSize: 17
                }

                Label {
                    id: label
                    x: padding + (isTreeNode ? (depth + 1) * indentation : 0)
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - padding - x - typeLabel.width
                    clip: true
                    text: model.name

                    color: Constants.palette.text

                    font.pixelSize: 17
                }

                Label {
                    id: typeLabel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    clip: true
                    text: isCamera ? "" : model.type

                    color: Constants.palette.text

                    font.pixelSize: 17
                }
            }
        }
    }

    Button {
        id: button2
        text: closedText

        width: !isCamera ? 40 : 0
        height: !isCamera ? 40 : 0

        onClicked: {
            if (text === closedText) {
                open()
                text = openText
            } else {
                close()
                text = closedText
            }
        }
    }
}
