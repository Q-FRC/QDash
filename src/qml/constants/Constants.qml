pragma Singleton

import QtQuick

QtObject {
    readonly property int width: 1280
    readonly property int height: 720

    property double scalar: 1.0

    property var palette: dark

    property color accent: "#ec202a"
    property color tab: "#ec5a5c"

    function setTheme(newTheme) {
        settings.theme = newTheme
        switch (newTheme) {
        case "light":
            palette = light
            break
        case "dark":
            palette = dark
            break
        case "midnight":
            palette = midnight
            break
        default:
            break
        }
    }

    function setAccent(newAccent) {
        settings.accent = newAccent

        accent = accents.accent(newAccent)
        tab = accents.light(newAccent)
    }

    property QtObject midnight: QtObject {
        id: midnight

        readonly property color bg: "#000000"
        readonly property color widgetBg: "#1B1B1B"
        readonly property color dialogBg: "#272727"

        readonly property color text: "#EEEEEE"
        readonly property color disabledText: Qt.darker(text, 1.75)

        readonly property color menu: "#111111"
        readonly property color menuBorder: "#AAAAAA"
        readonly property real menuBaseOpacity: 0.3
        readonly property color menuItem: bg

        readonly property color button: widgetBg
        readonly property color buttonHighlighted: "#4A4A4A"
    }

    property QtObject dark: QtObject {
        id: dark

        readonly property color bg: "#2d2d2d"
        readonly property color widgetBg: "#404040"
        readonly property color dialogBg: "#545454"

        readonly property color text: "#FFFFFF"
        readonly property color disabledText: Qt.darker(text, 2)

        readonly property color menu: "#595959"
        readonly property color menuBorder: "#AAAAAA"
        readonly property real menuBaseOpacity: 0.5
        readonly property color menuItem: bg

        readonly property color button: widgetBg
        readonly property color buttonHighlighted: "#6A6A6A"
    }

    property QtObject light: QtObject {
        readonly property color bg: "#FFFFFF"
        readonly property color widgetBg: "#EDEDED"
        readonly property color dialogBg: "#DCDCDC"

        readonly property color text: "#000000"
        readonly property color disabledText: "#666666"

        readonly property color menu: "#DDDDDD"
        readonly property color menuBorder: "#222222"
        readonly property real menuBaseOpacity: 0.8
        readonly property color menuItem: bg

        readonly property color button: widgetBg
        readonly property color buttonHighlighted: "#CCCCCC"
    }
}
