import QtQuick 6.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Shapes 2.15

import QFRCDashboard

BaseWidget {
    property string item_topic

    property bool item_useVerticalField: false
    property bool item_mirrorForRedAlliance: false

    property double item_robotWidthMeters: 0.5
    property double item_robotLengthMeters: 0.5

    property var item_field: "2024"

    property list<string> fieldChoices: ["2024", "2023"]

    property double fieldWidth: 8.2296;
    property double fieldLength: 8.2296 * 2.;

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
            let meterRatio = field.paintedHeight / fieldWidth

            height = item_robotWidthMeters * meterRatio
            width = item_robotLengthMeters * meterRatio

            let xPixels = xMeters * meterRatio
            let yPixels = yMeters * meterRatio

            let realFieldX = field.x + (field.width - field.paintedWidth) / 2
            let realFieldY = field.y + (field.height - field.paintedHeight) / 2

            let startPoint = item_useVerticalField ? Qt.point(realFieldX + field.paintedWidth - width, realFieldY + field.paintedHeight)
                  : Qt.point(realFieldX, realFieldY + field.paintedHeight)

            x = startPoint.x + xPixels
            y = startPoint.y - yPixels - height

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

                start.x =  w
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
}
