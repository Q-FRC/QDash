// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Rectangle {
    id: gauge

    property real angle: (value - minValue) / (maxValue - minValue) * (endAngle - startAngle) + startAngle
    property real endAngle: 135
    property real startAngle: -135

    property real maxValue: 99.4
    property real minValue: 0
    property real value: 1.80

    property int numTicks: 15
    property int valueFontSize: 20

    function fixGaugeSize() {
        if (width < height && width !== 0) {
            height = width
        } else if (height < width && height !== 0) {
            width = height
        }
    }

    color: "transparent"
    radius: width / 2

    onValueChanged: {
        if (value <= minValue)
            value = minValue
        if (value >= maxValue)
            value = maxValue
    }

    Item {
        id: container

        anchors {
            fill: parent
            margins: 8
        }

        Repeater {
            model: gauge.numTicks + (endAngle - startAngle >= 360 ? 0 : 1)

            Item {
                id: tickContainer

                height: container.height
                rotation: index * (gauge.endAngle - gauge.startAngle) / gauge.numTicks + gauge.startAngle
                width: container.width

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    spacing: txt.contentWidth / 2.5

                    Rectangle {
                        id: tick

                        anchors.horizontalCenter: parent.horizontalCenter
                        color: Clover.theme.text
                        height: 12
                        width: 2
                    }

                    Label {
                        id: txt

                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 11
                        rotation: -tickContainer.rotation
                        text: (gauge.minValue + index * (gauge.maxValue - gauge.minValue) / gauge.numTicks).toFixed(1)
                    }
                }
            }
        }

        Rectangle {
            id: knob

            anchors.centerIn: parent
            color: "lightgray"
            height: width
            radius: width / 2
            width: parent.width / 15
            z: 1
        }

        AcceleratedShape {
            id: needle

            anchors.bottom: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            height: gauge.width / 3
            rotation: gauge.angle
            transformOrigin: Item.Bottom
            width: knob.width

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
        AcceleratedShape {
            id: filled

            anchors.fill: parent
            z: 2

            // unfilled
            GaugePath {
                start: gauge.startAngle - 90
                stroke: gauge.angle >= gauge.endAngle ? "transparent" : "lightgray"
                sweep: gauge.endAngle - gauge.startAngle
            }

            // filled
            // draw it "over" the unfilled... because otherwise you get jank garbage
            // like weird gaps and such
            GaugePath {
                start: gauge.startAngle - 90
                stroke: Clover.theme.currentAccent
                sweep: gauge.angle - gauge.startAngle
            }
        }

        Label {
            font.pixelSize: valueFontSize
            text: gauge.value.toFixed(2)

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.verticalCenter
                topMargin: 20
            }
        }
    }

    component GaugePath: ShapePath {
        id: gaugePath

        required property int start
        required property color stroke
        required property int sweep

        capStyle: ShapePath.FlatCap
        fillColor: "transparent"
        startX: container.width / 2
        startY: container.height / 2
        strokeColor: stroke
        strokeWidth: 4

        PathAngleArc {
            centerX: container.width / 2
            centerY: container.height / 2
            radiusX: container.width / 2 - 2
            radiusY: container.height / 2 - 2
            startAngle: gaugePath.start
            sweepAngle: gaugePath.sweep
        }
    }
}
