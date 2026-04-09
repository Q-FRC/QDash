// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

import Carboxyl.Clover

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

    // 4x sampling, and antialiasing.
    layer.enabled: true
    layer.samples: 4
    layer.smooth: true
    antialiasing: true

    onValueChanged: {
        if (value <= minValue)
            value = minValue
        if (value >= maxValue)
            value = maxValue
    }

    color: "transparent"

    function fixGaugeSize() {
        if (width < height && width !== 0) {
            height = width
        } else if (height < width && height !== 0) {
            width = height
        }
    }

    radius: width / 2

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
                width: container.width
                height: container.height

                rotation: index * (gauge.endAngle - gauge.startAngle)
                          / gauge.numTicks + gauge.startAngle

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 5

                    spacing: txt.contentWidth / 2.5

                    Rectangle {
                        id: tick
                        width: 2
                        height: 12
                        color: Clover.theme.text
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Label {
                        id: txt

                        text: (gauge.minValue + index * (gauge.maxValue - gauge.minValue)
                               / gauge.numTicks).toFixed(1)

                        anchors.horizontalCenter: parent.horizontalCenter

                        rotation: -tickContainer.rotation
                        font.pixelSize: 11
                    }
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

        AcceleratedShape {
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
        AcceleratedShape {
            id: filled
            anchors.fill: parent
            z: 2

            component GaugePath: ShapePath {
                id: gaugePath

                required property int sweep
                required property int start
                required property color stroke

                capStyle: ShapePath.FlatCap

                strokeWidth: 4
                strokeColor: stroke
                fillColor: "transparent"

                startX: container.width / 2
                startY: container.height / 2

                PathAngleArc {
                    radiusX: container.width / 2 - 2
                    radiusY: container.height / 2 - 2
                    centerX: container.width / 2
                    centerY: container.height / 2
                    startAngle: gaugePath.start
                    sweepAngle: gaugePath.sweep
                }
            }

            // unfilled
            GaugePath {
                stroke: gauge.angle >= gauge.endAngle ? "transparent" : "lightgray"

                start: gauge.startAngle - 90
                sweep: gauge.endAngle - gauge.startAngle
            }

            // filled
            // draw it "over" the unfilled... because otherwise you get jank garbage
            // like weird gaps and such
            GaugePath {
                stroke: Clover.theme.currentAccent

                start: gauge.startAngle - 90
                sweep: gauge.angle - gauge.startAngle
            }
        }

        Label {
            text: gauge.value.toFixed(2)
            font.pixelSize: valueFontSize

            anchors {
                top: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
                topMargin: 20
            }
        }
    }
}
