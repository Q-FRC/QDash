pragma Singleton
import QtQuick

QtObject {
    readonly property int width: 900;
    readonly property int height: 600;

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
        switch (newAccent) {
        case "red":
            accent = "#ec202a"
            tab = "#ec5a5c"
            break
        case "blue":
            accent = "#0024c2"
            tab = "#4520ff"
            break
        case "purple":
            accent = "#a100a1"
            tab = "#b400e1"
            break
        default:
            break;
        }
    }

    property QtObject midnight: QtObject {
        id: midnight

        readonly property color bg: "#000000";
        readonly property color widgetBg: "#1B1B1B";
        readonly property color dialogBg: "#272727";

        readonly property color text: "#EEEEEE"

        readonly property color menu: "#111111"
        readonly property color menuBorder: "#AAAAAA"
        readonly property real menuBaseOpacity: 0.3
        readonly property color menuItem: bg
    }

    property QtObject dark: QtObject {
        id: dark

        readonly property color bg: "#4d4d4d";
        readonly property color widgetBg: "#5d5d5d";
        readonly property color dialogBg: "#717171";

        readonly property color text: "#FFFFFF"

        readonly property color menu: "#595959"
        readonly property color menuBorder: "#AAAAAA"
        readonly property real menuBaseOpacity: 0.5
        readonly property color menuItem: bg
    }

    property QtObject light: QtObject {
        readonly property color bg: "#FFFFFF";
        readonly property color widgetBg: "#EDEDED";
        readonly property color dialogBg: "#DCDCDC";

        readonly property color text: "#000000"

        readonly property color menu: "#DDDDDD"
        readonly property color menuBorder: "#222222"
        readonly property real menuBaseOpacity: 0.8
        readonly property color menuItem: bg
    }
}
