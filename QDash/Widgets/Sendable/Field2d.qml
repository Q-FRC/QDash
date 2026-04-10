// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 6.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4
import QtQuick.Shapes 2.15

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    suffix: "/Robot"

    antialiasing: true
    propertyKeys: ["robotShape", "robotColor", "robotWidthMeters", "robotLengthMeters", "useVerticalField", "mirrorForRedAlliance", "fieldType"]

    property bool useVerticalField: false
    property bool mirrorForRedAlliance: false

    property double robotWidthMeters: 0.5
    property double robotLengthMeters: 0.5

    property string fieldType: "2026"

    property list<string> fieldChoices: ["2026", "2025", "2024", "2023"]

    property double fieldWidth: 8.0692752
    property double fieldLength: 16.5411912

    property bool mirrorField: false

    property string robotShape: "Robot"

    property list<string> robotShapeChoices: ["Robot", "Circle", "Rectangle"]

    property color robotColor: "#FF0000"

    function redraw() {
        robot.redraw()
    }

    onRobotLengthMetersChanged: redraw()
    onRobotWidthMetersChanged: redraw()

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

    function unsubscribeMirror() {
        if (TopicStore !== null) {
            TopicStore.unsubscribe("/FMSInfo/IsRedAlliance", updateMirror)
        }
    }

    onMirrorForRedAllianceChanged: {
        if (mirrorForRedAlliance) {
            TopicStore.subscribe("/FMSInfo/IsRedAlliance", updateMirror)
        } else {
            unsubscribeMirror()
            mirrorField = false
        }
    }

    Component.onDestruction: unsubscribeMirror()

    Image {
        id: fieldImage

        y: titleField.height + 10
        x: 8

        width: parent.width - 16
        height: parent.height - titleField.height - 16

        fillMode: Image.PreserveAspectFit
        source: "qrc:/" + fieldType + "Field" + (useVerticalField ? "Vertical" : "") + ".png"
        onSourceChanged: robot.redraw()

        onPaintedGeometryChanged: robot.redraw()

        mirrorVertically: useVerticalField ? mirrorField : false
        mirror: useVerticalField ? false : mirrorField
    }

    Rectangle {
        id: robot

        color: robotShape === "Robot" ? "transparent" : robotColor
        border {
            color: robotColor
            width: 3
        }

        radius: robotShape === "Circle" ? Math.max(width, height) / 2 : 0

        property double xMeters: 0
        property double yMeters: 0
        property double angleDeg: 0

        function redraw() {
            let meterRatio = (useVerticalField ? fieldImage.paintedWidth : fieldImage.paintedHeight)
                / fieldWidth

            height = robotWidthMeters * meterRatio
            width = robotLengthMeters * meterRatio

            let xPixels = (useVerticalField ? -yMeters : xMeters) * meterRatio
            let yPixels = (useVerticalField ? xMeters : yMeters) * meterRatio

            let realFieldX = fieldImage.x + (fieldImage.width - fieldImage.paintedWidth) / 2
            let realFieldY = fieldImage.y + (fieldImage.height - fieldImage.paintedHeight) / 2

            let startPoint = useVerticalField ? Qt.point(
                                                    realFieldX + fieldImage.paintedWidth - width,
                                                    realFieldY
                                                    + fieldImage.paintedHeight) : Qt.point(
                                                    realFieldX,
                                                    realFieldY + fieldImage.paintedHeight)

            x = startPoint.x + xPixels - (useVerticalField ? -height : width) / 2
            y = startPoint.y - yPixels - (useVerticalField ? width : height) / 2

            rotation = -angleDeg + (useVerticalField ? 270 : 0)

            path.redraw(x, y, height, width, angleDeg)
        }
    }

    AcceleratedShape {
        id: shape
        z: 2
        visible: robotShape === "Robot"

        ShapePath {
            id: path
            strokeWidth: 4
            strokeColor: "light green"
            fillColor: "transparent"

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

                middle.x = 2
                middle.y = h - 2

                end.x = 2
                end.y = 0

                shape.rotation = -rot + (useVerticalField ? 270 : 0)
            }
        }
    }

    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: Component {
            BaseConfigDialog {
                id: config

                content: ColumnLayout {
                    id: layout
                    spacing: 12
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

                        bindedProperty: "titleFontSize"
                    }

                    SectionHeader {
                        label: "Robot Settings"
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        LabeledComboBox {
                            id: robotShapeField

                            Layout.fillWidth: true

                            label: "Robot Shape"

                            bindedProperty: "robotShape"

                            model: robotShapeChoices
                        }

                        ColorField {
                            id: colorField

                            Layout.fillWidth: true

                            label: "Robot Color"

                            bindedProperty: "robotColor"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        LabeledDoubleSpinBox {
                            id: robotWField

                            Layout.fillWidth: true
                            from: 0

                            label: "Robot Width (m)"

                            bindedProperty: "robotWidthMeters"

                            stepSize: 0.1
                        }

                        LabeledDoubleSpinBox {
                            id: robotLField

                            Layout.fillWidth: true
                            from: 0

                            label: "Robot Length (m)"

                            bindedProperty: "robotLengthMeters"

                            stepSize: 0.1
                        }
                    }

                    SectionHeader {
                        label: "Field Settings"
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        LabeledCheckbox {
                            id: vertField
                            Layout.fillWidth: true

                            label: "Use Vertical Field"

                            bindedProperty: "useVerticalField"
                        }

                        LabeledCheckbox {
                            id: mirrorRedField
                            Layout.fillWidth: true

                            label: "Mirror for Red"

                            bindedProperty: "mirrorForRedAlliance"
                        }
                    }

                    LabeledComboBox {
                        id: fieldField // lol
                        Layout.fillWidth: true

                        label: "Field Type"
                        model: fieldChoices

                        bindedProperty: "fieldType"
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
                    }
                }
            }
        }
    }
}
