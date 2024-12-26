import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

ColumnLayout {
    spacing: 5

    function editAccents() {
        accentEditor.open()
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

    function exportAccents() {
        saveAccentDialog.open()
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

    function importAccents() {
        loadAccentDialog.open()
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
            implicitWidth: 200
            id: theme

            label: "Theme"

            bindedProperty: "theme"
            bindTarget: settings

            choices: ["Light", "Dark", "Midnight"]
        }

        AppearanceComboBox {
            implicitWidth: 200
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
            text: "&Edit"
            onClicked: editAccents()
        }
        Button {
            Layout.fillWidth: true
            text: "E&xport"
            onClicked: exportAccents()
        }
        Button {
            Layout.fillWidth: true
            text: "I&mport"
            onClicked: importAccents()
        }
    }
}
