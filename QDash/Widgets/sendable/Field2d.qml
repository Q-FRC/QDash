// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QDash.Controls

import QDash.Core
import QDash.Widgets
import QtQuick 6.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8
import QtQuick.Shapes 2.15

PrimitiveWidget {
    id: widget

    property list<string> fieldChoices: ["2026", "2025", "2024", "2023"]
    property double fieldLength: 16.5411912
    property string fieldType: "2026"
    property double fieldWidth: 8.0692752
    property bool mirrorField: false
    property bool mirrorForRedAlliance: false
    property color robotColor: "#FF0000"
    property double robotLengthMeters: 0.5
    property string robotShape: "Robot"
    property list<string> robotShapeChoices: ["Robot", "Circle", "Rectangle"]
    property double robotWidthMeters: 0.5
    property bool useVerticalField: false

    function redraw() {
        robot.redraw()
    }

    function unsubscribeMirror() {
        if (TopicStore !== null) {
            TopicStore.unsubscribe("/FMSInfo/IsRedAlliance", updateMirror)
        }
    }

    function update(value) {
        robot.xMeters = value[0]
        robot.yMeters = value[1]
        robot.angleDeg = value[2]

        redraw()
    }

    // TODO: This is kinda weird
    // might want to make this global or something? idk
    function updateMirror(value) {
        mirrorField = value
    }

    antialiasing: true
    propertyKeys: ["robotShape", "robotColor", "robotWidthMeters", "robotLengthMeters", "useVerticalField", "mirrorForRedAlliance", "fieldType"]
    suffix: "/Robot"

    configContent: ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true
        spacing: 12

        SectionHeader {
            label: "Font Settings"
        }

        LabeledSpinBox {
            bindedProperty: "titleFontSize"
            label: "Title Font Size"
        }

        SectionHeader {
            label: "Robot Settings"
        }

        RowLayout {
            Layout.fillWidth: true

            LabeledComboBox {
                bindedProperty: "robotShape"
                label: "Robot Shape"
                model: robotShapeChoices
            }

            ColorField {
                bindedProperty: "robotColor"
                label: "Robot Color"
            }
        }

        RowLayout {
            Layout.fillWidth: true

            LabeledDoubleSpinBox {
                bindedProperty: "robotWidthMeters"
                from: 0
                label: "Robot Width (m)"
                stepSize: 0.1
            }

            LabeledDoubleSpinBox {
                bindedProperty: "robotLengthMeters"
                from: 0
                label: "Robot Length (m)"
                stepSize: 0.1
            }
        }

        SectionHeader {
            label: "Field Settings"
        }

        RowLayout {
            Layout.fillWidth: true

            LabeledCheckbox {
                bindedProperty: "useVerticalField"
                label: "Use Vertical Field"
            }

            LabeledCheckbox {
                bindedProperty: "mirrorForRedAlliance"
                label: "Mirror for Red"
            }
        }

        LabeledComboBox {
            bindedProperty: "fieldType"
            label: "Field Type"
            model: fieldChoices
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
        }
    }

    Component.onDestruction: unsubscribeMirror()
    onMirrorForRedAllianceChanged: {
        if (mirrorForRedAlliance) {
            TopicStore.subscribe("/FMSInfo/IsRedAlliance", updateMirror)
        } else {
            unsubscribeMirror()
            mirrorField = false
        }
    }
    onRobotLengthMetersChanged: redraw()
    onRobotWidthMetersChanged: redraw()

    Image {
        id: fieldImage

        fillMode: Image.PreserveAspectFit
        height: parent.height - titleField.height - 16
        mirror: useVerticalField ? false : mirrorField
        mirrorVertically: useVerticalField ? mirrorField : false
        source: "qrc:/" + fieldType + "Field" + (useVerticalField ? "Vertical" : "") + ".png"
        width: parent.width - 16
        x: 8
        y: titleField.height + 10

        onPaintedGeometryChanged: robot.redraw()
        onSourceChanged: robot.redraw()
    }

    Rectangle {
        id: robot

        property double angleDeg: 0
        property double xMeters: 0
        property double yMeters: 0

        function redraw() {
            let meterRatio = (useVerticalField ? fieldImage.paintedWidth : fieldImage.paintedHeight) / fieldWidth

            height = robotWidthMeters * meterRatio
            width = robotLengthMeters * meterRatio

            let xPixels = (useVerticalField ? -yMeters : xMeters) * meterRatio
            let yPixels = (useVerticalField ? xMeters : yMeters) * meterRatio

            let realFieldX = fieldImage.x + (fieldImage.width - fieldImage.paintedWidth) / 2
            let realFieldY = fieldImage.y + (fieldImage.height - fieldImage.paintedHeight) / 2

            let startPoint = useVerticalField ? Qt.point(realFieldX + fieldImage.paintedWidth - width, realFieldY + fieldImage.paintedHeight) : Qt.point(realFieldX, realFieldY + fieldImage.paintedHeight)

            x = startPoint.x + xPixels - (useVerticalField ? -height : width) / 2
            y = startPoint.y - yPixels - (useVerticalField ? width : height) / 2

            rotation = -angleDeg + (useVerticalField ? 270 : 0)

            path.redraw(x, y, height, width, angleDeg)
        }

        color: robotShape === "Robot" ? "transparent" : robotColor
        radius: robotShape === "Circle" ? Math.max(width, height) / 2 : 0

        border {
            color: robotColor
            width: 3
        }
    }

    AcceleratedShape {
        id: shape

        visible: robotShape === "Robot"
        z: 2

        ShapePath {
            id: path

            function redraw(x, y, h, w, rot) {
                shape.x = x
                shape.y = y

                shape.width = w
                shape.height = h

                start.x = w
                start.y = h / 2

                middle.x = 2
                middle.y = h - 2

                end.x = 2
                end.y = 0

                shape.rotation = -rot + (useVerticalField ? 270 : 0)
            }

            fillColor: "transparent"
            strokeColor: "light green"
            strokeWidth: 4

            PathLine {
                id: start
            }

            PathLine {
                id: middle
            }

            PathLine {
                id: end
            }
        }
    }
}
