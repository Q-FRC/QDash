import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import constants
import config

ColumnLayout {
    spacing: 5

    Component.onCompleted: {
        accentEditor.accepted.connect(() => accent.choices = accents.names())
    }

    FileDialog {
        id: saveAccentDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.SaveFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: accents.exportJson(selectedFile)
    }

    FileDialog {
        id: loadAccentDialog
        currentFolder: StandardPaths.writableLocation(
                           StandardPaths.HomeLocation)
        fileMode: FileDialog.OpenFile
        defaultSuffix: "json"
        selectedNameFilter.index: 0
        nameFilters: ["JSON files (*.json)", "All files (*)"]

        onAccepted: accents.importJson(loadAccentDialog.selectedFile)
    }

    function accept() {
        theme.accept()
        accent.accept()

        Constants.setTheme(settings.theme)
        Constants.setAccent(settings.accent)
    }

    function open() {
        theme.open()
        accent.open()
    }

    RowLayout {
        Layout.fillWidth: true

        AppearanceComboBox {
            implicitHeight: 50 * Constants.scalar
            implicitWidth: 200 * Constants.scalar
            id: theme

            label: "Theme"

            bindedProperty: "theme"
            bindTarget: settings

            choices: ["Light", "Dark", "Midnight"]
        }

        AppearanceComboBox {
            implicitWidth: 200 * Constants.scalar
            implicitHeight: 50 * Constants.scalar
            id: accent

            label: "Accent"

            bindedProperty: "accent"
            bindTarget: settings

            choices: accents.names()
        }
    }

    SectionHeader {
        label: "Custom Accents"
    }

    RowLayout {
        uniformCellSizes: true
        Layout.fillWidth: true

        Button {
            Layout.fillWidth: true

            font.pixelSize: 18 * Constants.scalar
            text: "&Edit"
            onClicked: accentEditor.open()
        }
        Button {
            Layout.fillWidth: true

            font.pixelSize: 18 * Constants.scalar
            text: "E&xport"
            onClicked: saveAccentDialog.open()
        }
        Button {
            Layout.fillWidth: true

            font.pixelSize: 18 * Constants.scalar
            text: "I&mport"
            onClicked: loadAccentDialog.open()
        }
    }
}
