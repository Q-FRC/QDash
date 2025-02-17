import QtQuick 6.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Shapes 2.15

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property bool item_useVerticalField: false
    property bool item_mirrorForRedAlliance: false

    property double item_robotWidthMeters: 0.5
    property double item_robotLengthMeters: 0.5

    property var item_field: "2025"

    property list<string> fieldChoices: ["2025", "2024", "2023"]

    property double fieldWidth: 8.051902
    property double fieldLength: 17.54825

    property bool mirrorField: false

    property var item_robotShape: "Robot"

    property list<string> robotShapeChoices: ["Robot", "Circle", "Rectangle"]

    property color item_robotColor: "#FF0000"

    function redraw() {
        robot.redraw()
    }

    onItem_robotLengthMetersChanged: redraw()
    onItem_robotWidthMetersChanged: redraw()

    function updateMirror(topic, value) {
        if (topic === "/FMSInfo/IsRedAlliance") {
            mirrorField = value
        }
    }

    function unsubscribeMirror() {
        if (topicStore !== null) {
            topicStore.topicUpdate.disconnect(updateMirror)
            topicStore.unsubscribe("/FMSInfo/IsRedAlliance")
        }
    }

    onItem_mirrorForRedAllianceChanged: {
        if (item_mirrorForRedAlliance) {
            topicStore.topicUpdate.connect(updateMirror)
            topicStore.subscribe("/FMSInfo/IsRedAlliance")
            mirrorField = topicStore.getValue("/FMSInfo/IsRedAlliance")
        } else {
            unsubscribeMirror()
            mirrorField = false
        }
    }

    Component.onDestruction: {
        unsubscribeMirror()
    }

    Image {
        id: field

        y: titleField.height + 10
        x: 8

        width: parent.width - 16
        height: parent.height - titleField.height - 16

        fillMode: Image.PreserveAspectFit
        source: "qrc:/" + item_field + "Field" + (item_useVerticalField ? "Vertical" : "") + ".png"
        onSourceChanged: robot.redraw()

        onPaintedGeometryChanged: robot.redraw()

        mirrorVertically: item_useVerticalField ? mirrorField : false
        mirror: item_useVerticalField ? false : mirrorField
    }

    Rectangle {
        color: item_robotShape === "Robot" ? "transparent" : item_robotColor
        border {
            color: item_robotColor
            width: 2
        }

        radius: item_robotShape === "Circle" ? Math.max(width, height) / 2 : 0

        property double xMeters: 0
        property double yMeters: 0
        property double angleDeg: 0

        id: robot

        function updateTopic(ntTopic, value) {
            if (ntTopic === item_topic + "/Robot") {
                xMeters = value[0]
                yMeters = value[1]
                angleDeg = value[2]
                redraw()
            }
        }

        function redraw() {
            let meterRatio = (item_useVerticalField ? field.paintedWidth : field.paintedHeight)
                / fieldWidth

            height = item_robotWidthMeters * meterRatio
            width = item_robotLengthMeters * meterRatio

            let xPixels = (item_useVerticalField ? -yMeters : xMeters) * meterRatio
            let yPixels = (item_useVerticalField ? xMeters : yMeters) * meterRatio

            let realFieldX = field.x + (field.width - field.paintedWidth) / 2
            let realFieldY = field.y + (field.height - field.paintedHeight) / 2

            let startPoint = item_useVerticalField ? Qt.point(
                                                         realFieldX + field.paintedWidth - width,
                                                         realFieldY
                                                         + field.paintedHeight) : Qt.point(
                                                         realFieldX,
                                                         realFieldY + field.paintedHeight)

            x = startPoint.x + xPixels - (item_useVerticalField ? -height : width) / 2
            y = startPoint.y - yPixels - (item_useVerticalField ? width : height) / 2

            rotation = -angleDeg + (item_useVerticalField ? 270 : 0)

            path.redraw(x, y, height, width, angleDeg)
        }

        function update() {
            topicStore.subscribe(item_topic + "/Robot")
        }

        function unsubscribe() {
            topicStore.unsubscribe(topic + "/Robot")
        }

        Component.onCompleted: {
            topicStore.topicUpdate.connect(updateTopic)
            item_topic = model.topic

            update()
        }

        Component.onDestruction: {
            if (topicStore !== null) {
                topicStore.topicUpdate.disconnect(updateTopic)
                unsubscribe()
            }
        }
    }

    Shape {
        id: shape

        ShapePath {
            id: path
            strokeWidth: 3
            strokeColor: item_robotShape === "Robot" ? "light green" : "transparent"

            PathLine {
                id: start
            }
            PathLine {
                id: middle
            }
            PathLine {
                id: end
            }

            function redraw(x, y, h, w, rot) {
                shape.x = x
                shape.y = y

                shape.width = w
                shape.height = h

                start.x = w
                start.y = h / 2

                middle.x = 0
                middle.y = h

                end.x = 0
                end.y = 0

                shape.rotation = -rot + (item_useVerticalField ? 270 : 0)
            }
        }
    }

    onItem_topicChanged: {
        robot.unsubscribe()

        model.topic = item_topic

        robot.update()
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            robotShapeField.open()
            colorField.open()
            robotWField.open()
            robotLField.open()
            vertField.open()
            mirrorRedField.open()
            fieldField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            robotShapeField.accept()
            colorField.accept()
            robotWField.accept()
            robotLField.accept()
            vertField.accept()
            mirrorRedField.accept()
            fieldField.accept()
        }

        ScrollView {
            clip: true

            contentWidth: width - 5 * Constants.scalar - effectiveScrollBarWidth

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: 5 * Constants.scalar

                rightMargin: 5
            }

            ColumnLayout {
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
                    label: "Robot Settings"
                }

                RowLayout {
                    Layout.fillWidth: true
                    uniformCellSizes: true

                    LabeledComboBox {
                        id: robotShapeField

                        Layout.fillWidth: true

                        label: "Robot Shape"

                        bindedProperty: "item_robotShape"
                        bindTarget: widget

                        choices: robotShapeChoices
                    }

                    ColorField {
                        id: colorField

                        Layout.fillWidth: true

                        label: "Robot Color"

                        bindedProperty: "item_robotColor"
                        bindTarget: widget
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    uniformCellSizes: true

                    LabeledDoubleSpinBox {
                        id: robotWField

                        Layout.fillWidth: true
                        from: 0

                        label: "Robot Width (m)"

                        bindedProperty: "item_robotWidthMeters"
                        bindTarget: widget

                        stepSize: 0.1
                    }

                    LabeledDoubleSpinBox {
                        id: robotLField

                        Layout.fillWidth: true
                        from: 0

                        label: "Robot Length (m)"

                        bindedProperty: "item_robotLengthMeters"
                        bindTarget: widget

                        stepSize: 0.1
                    }
                }

                SectionHeader {
                    label: "Field Settings"
                }

                RowLayout {
                    uniformCellSizes: true
                    Layout.fillWidth: true

                    LabeledCheckbox {
                        id: vertField
                        Layout.fillWidth: true

                        label: "Use Vertical Field"

                        bindedProperty: "item_useVerticalField"
                        bindTarget: widget
                    }

                    LabeledCheckbox {
                        id: mirrorRedField
                        Layout.fillWidth: true

                        label: "Mirror for Red"

                        bindedProperty: "item_mirrorForRedAlliance"
                        bindTarget: widget
                    }
                }

                LabeledComboBox {
                    id: fieldField // lol
                    Layout.fillWidth: true

                    label: "Field Type"
                    choices: fieldChoices

                    bindedProperty: "item_field"
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
}
