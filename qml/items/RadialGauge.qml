import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

import QFRCDashboard

Rectangle {
    id: gauge

    property int numTicks: 15
    property real value: 1.80
    property real minValue: 0
    property real maxValue: 99.4
    property real startAngle: -135
    property real endAngle: 135

    property real angle: (value - minValue) / (maxValue - minValue)
                         * (endAngle - startAngle) + startAngle

    property int valueFontSize: 20

    onValueChanged: {
        if (value <= minValue)
            value = minValue
        if (value >= maxValue)
            value = maxValue
    }

    color: "transparent"

    function fixSize() {
        if (width < height && width !== 0) {
            height = width
        } else if (height < width && height !== 0) {
            width = height
        }
    }

    radius: width / 2

    Repeater {
        model: gauge.numTicks + 1

        Item {
            width: gauge.width
            height: gauge.height

            rotation: index * (gauge.endAngle - gauge.startAngle)
                      / gauge.numTicks + gauge.startAngle

            Rectangle {
                id: tick
                width: 2 * Constants.scalar
                height: 10 * Constants.scalar

                color: Constants.palette.text

                anchors {
                    top: parent.top
                    topMargin: 10 * Constants.scalar
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                color: Constants.palette.text

                text: (gauge.minValue + index * (gauge.maxValue - gauge.minValue)
                       / gauge.numTicks).toFixed(1)

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: tick.bottom
                    topMargin: 5 * Constants.scalar
                }

                rotation: -parent.rotation
            }
        }
    }

    Rectangle {
        id: knob
        color: "lightgray"

        width: parent.width / 15
        height: width

        radius: width / 2

        anchors.centerIn: parent

        z: 1
    }

    Shape {
        id: needle

        anchors.bottom: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        rotation: gauge.angle
        transformOrigin: Item.Bottom

        width: knob.width
        height: gauge.width / 3

        ShapePath {
            fillColor: "red"

            startX: needle.width / 2
            startY: 0

            PathLine {
                x: needle.width * 3 / 4
                y: needle.height
            }

            PathLine {
                x: needle.width * 1 / 4
                y: needle.height
            }

            PathLine {
                x: needle.width / 2
                y: 0
            }
        }
    }

    // Filled
    Shape {
        id: filled
        anchors.fill: parent

        ShapePath {
            strokeWidth: 5 * Constants.scalar
            strokeColor: "blue"
            fillColor: "transparent"

            capStyle: ShapePath.FlatCap

            startX: gauge.width / 2
            startY: gauge.height / 2

            PathAngleArc {
                radiusX: gauge.width / 2 - 2
                radiusY: gauge.height / 2 - 2
                centerX: gauge.width / 2
                centerY: gauge.height / 2
                startAngle: gauge.startAngle - 90
                sweepAngle: gauge.angle - gauge.startAngle
            }
        }
    }

    // Unfilled
    Shape {
        id: unfilled
        anchors.fill: parent

        ShapePath {
            strokeWidth: 3 * Constants.scalar
            strokeColor: gauge.angle >= gauge.endAngle ? "transparent" : "lightgray"
            fillColor: "transparent"

            capStyle: ShapePath.FlatCap

            startX: gauge.width / 2
            startY: gauge.height / 2

            PathAngleArc {
                radiusX: gauge.width / 2 - 2
                radiusY: gauge.height / 2 - 2
                centerX: gauge.width / 2
                centerY: gauge.height / 2
                startAngle: gauge.angle - 90
                sweepAngle: gauge.endAngle - gauge.angle
            }
        }
    }
    Text {
        text: gauge.value.toFixed(2)
        color: "white"
        font.pixelSize: valueFontSize

        anchors {
            top: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
            topMargin: 20 * Constants.scalar
        }
    }
}
