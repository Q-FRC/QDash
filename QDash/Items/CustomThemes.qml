pragma Singleton

import QtQuick
import Carboxyl.Clover

Item {
    property alias hannahDark: hannahDark

    CloverPalette {
        id: hannahDark

        name: "Hannah Montana"
        currentAccent: Clover.accent.dark

        active {
            base: "#1a0524"
            alternateBase: "#2d0b3d"
            button: "#3e1e4a"

            text: "#f5e6ff"
            highlightedText: "#ffffff"
            buttonText: "#ffffff"

            brightText: "#ffe100"

            dark: "#0f0214"

            light: "#bd93f9"

            mid: "#4b1d63"

            midlight: "#6e338c"

            link: "#00f2ff"
            linkVisited: "#e0b0ff"

            shadow: "#050108"

            toolTipBase: "#120a21"
            toolTipText: "#f5e6ff"

            window: "#14031c"
            windowText: "#f5e6ff"

            placeholderText: "#dcc6ff"
        }

        inactive {
            alternateBase: CarboxylAlwaysActive ? active.alternateBase : "#261130"
            base: CarboxylAlwaysActive ? active.base : "#18091f"
            button: CarboxylAlwaysActive ? active.button : "#3e1e4a"

            text: CarboxylAlwaysActive ? active.text : "#8a7a91"
            highlightedText: CarboxylAlwaysActive ? active.highlightedText : "#8a7a91"
            buttonText: CarboxylAlwaysActive ? active.buttonText : "#8a7a91"

            brightText: CarboxylAlwaysActive ? active.brightText : "#ffffff"

            dark: CarboxylAlwaysActive ? active.dark : "#0f0214"
            light: CarboxylAlwaysActive ? active.light : "#4b1d63"
            mid: CarboxylAlwaysActive ? active.mid : "#240b30"
            midlight: CarboxylAlwaysActive ? active.midlight : "#351a42"

            link: CarboxylAlwaysActive ? active.link : "#59d0d6"
            linkVisited: CarboxylAlwaysActive ? active.linkVisited : "#a983c2"

            shadow: CarboxylAlwaysActive ? active.shadow : "#050108"
            toolTipBase: CarboxylAlwaysActive ? active.toolTipBase : "#120a21"
            toolTipText: CarboxylAlwaysActive ? active.toolTipText : "#f5e6ff"

            window: CarboxylAlwaysActive ? active.window : "#1c0a24"
            windowText: CarboxylAlwaysActive ? active.windowText : "#8a7a91"

            placeholderText: CarboxylAlwaysActive ? active.placeholderText : "#5e5263"
        }

        disabled {
            base: "#140d1a"
            alternateBase: "#1c1424"
            button: "#2d2433"

            text: "#5c5061"
            highlightedText: "#5c5061"
            buttonText: "#5c5061"

            brightText: "#ffffff"

            dark: "#0f0214"
            light: "#3d3445"
            mid: "#1a141f"
            midlight: "#3a2f42"

            link: "#718e9e"
            linkVisited: "#8c7a94"

            shadow: "#050108"
            toolTipBase: "#120a21"
            toolTipText: "#f5e6ff"

            window: "#1a1221"
            windowText: "#5c5061"

            placeholderText: "#332c38"
        }
    }
}
