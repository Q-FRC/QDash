import QtCore
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs

import dialogs
import constants
import items
import logging

ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height
    visible: true
    title: conn.title

    flags: Qt.FramelessWindowHint | Qt.Window

    Material.theme: settings.theme === "light" ? Material.Light : Material.Dark
    Material.accent: accents.qml(
                         settings.accent) // "qml" is the Material Theme accent. Affects checkboxes, etc.

    Material.roundedScale: Material.NotRounded

    property string filename: ""
    property int minWidth: 250
    property int minHeight: 250

    function resetScalar() {
        Constants.scalar = Math.sqrt(
                    Math.min(width / Constants.width,
                             height / Constants.height)) * settings.scale

        logs.debug("UI", "Scalar reset to " + Constants.scalar)
    }

    function dsResize() {
        if (settings.resizeToDS) {
            logs.debug("UI", "DS Resize")

            height = platformHelper.screenHeight() - 236
            width = platformHelper.screenWidth()

            x = 0
            y = 0
        }
    }

    onWidthChanged: {
        resetScalar()
    }

    onHeightChanged: {
        resetScalar()
    }

    // Dialogs
    AccentEditor {
        id: accentEditor
    }

    NotificationPopup {
        id: notif

        x: parent.width - width - 10
        y: parent.height - height - 10
    }

    /** SAVE */
    FileDialog {
        id: saveDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.SaveFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: saveAs()
    }

    function save() {
        if (filename === "")
            return saveDialog.open()

        tlm.save(filename)
    }

    function saveAs() {
        filename = saveDialog.selectedFile

        tlm.save(filename)
    }

    /** LOAD */
    FileDialog {
        id: loadDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.OpenFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: load()
    }

    function load() {
        filename = loadDialog.selectedFile
        logs.info("IO", "Loading file " + filename)
        tlm.load(filename)
    }

    /** SERVER SETTINGS */
    SettingsDialog {
        id: settingsDialog
    }

    /** TITLE BAR */
    TitleBar {
        id: titleBar

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        onMinimize: window.visibility = Window.Minimized
        onMaximize: {
            if (window.visibility === Window.Maximized) {
                window.visibility = Window.Windowed
            } else {
                window.visibility = Window.Maximized
            }
        }
        onClose: window.close()
        onBeginDrag: window.startSystemMove()

        onAddWidget: (title, topic, type) => screen.addWidget(title,
                                                              topic, type)
    }

    /** THE REST */
    Component.onCompleted: {
        Constants.setTheme(settings.theme)
        Constants.setAccent(settings.accent)

        resetScalar()
        settings.scaleChanged.connect(resetScalar)

        dsResize()
        settings.resizeToDSChanged.connect(dsResize)

        if (settings.loadRecent && settings.recentFiles.length > 0) {
            filename = settings.recentFiles[0]
            if (filename === "" || filename === null)
                return
            tlm.load(filename)
        }
    }

    MainScreen {
        id: screen
        anchors {
            top: titleBar.bottom
            left: parent.left
            right: parent.right
            bottom: toolbar.top
        }
    }

    /** Status Bar */
    // TODO: Remove leading file:///
    ToolBar {
        id: toolbar

        background: Rectangle {
            color: Constants.accent
        }

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        implicitHeight: 30 * Constants.scalar
        Text {
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter

                leftMargin: 20 * Constants.scalar
            }

            text: "Status: " + conn.status

            color: Constants.palette.text
            font.pixelSize: 16 * Constants.scalar
        }

        Text {
            anchors {
                centerIn: parent
            }

            // TODO: Remove file://
            text: filename === "" ? "No File" : filename

            color: Constants.palette.text
            font.pixelSize: 16 * Constants.scalar
        }
    }

    /* RESIZE ANCHORS */
    Repeater {
        model: [Qt.RightEdge, Qt.LeftEdge, Qt.TopEdge, Qt.BottomEdge, Qt.RightEdge
            | Qt.TopEdge, Qt.RightEdge | Qt.BottomEdge, Qt.LeftEdge
            | Qt.TopEdge, Qt.LeftEdge | Qt.BottomEdge]

        ResizeAnchor {
            required property int modelData
            direction: modelData

            mouseArea.onPressed: window.startSystemResize(direction)
            mouseArea.drag.target: null

            divisor: 80 / Constants.scalar
        }
    }
}
