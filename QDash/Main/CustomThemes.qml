// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton
import Carboxyl.Clover

import QtQuick

Item {
    property alias hannahDark: hannahDark

    CloverPalette {
        id: hannahDark

        currentAccent: Clover.accent.dark
        name: "Hannah Montana"

        active {
            alternateBase: "#2d0b3d"
            base: "#1a0524"
            brightText: "#ffe100"
            button: "#3e1e4a"
            buttonText: "#ffffff"
            dark: "#0f0214"
            highlightedText: "#ffffff"
            light: "#bd93f9"
            link: "#00f2ff"
            linkVisited: "#e0b0ff"
            mid: "#4b1d63"
            midlight: "#6e338c"
            placeholderText: "#dcc6ff"
            shadow: "#050108"
            text: "#f5e6ff"
            toolTipBase: "#120a21"
            toolTipText: "#f5e6ff"
            window: "#14031c"
            windowText: "#f5e6ff"
        }

        inactive {
            alternateBase: CarboxylAlwaysActive ? active.alternateBase : "#261130"
            base: CarboxylAlwaysActive ? active.base : "#18091f"
            brightText: CarboxylAlwaysActive ? active.brightText : "#ffffff"
            button: CarboxylAlwaysActive ? active.button : "#3e1e4a"
            buttonText: CarboxylAlwaysActive ? active.buttonText : "#8a7a91"
            dark: CarboxylAlwaysActive ? active.dark : "#0f0214"
            highlightedText: CarboxylAlwaysActive ? active.highlightedText : "#8a7a91"
            light: CarboxylAlwaysActive ? active.light : "#4b1d63"
            link: CarboxylAlwaysActive ? active.link : "#59d0d6"
            linkVisited: CarboxylAlwaysActive ? active.linkVisited : "#a983c2"
            mid: CarboxylAlwaysActive ? active.mid : "#240b30"
            midlight: CarboxylAlwaysActive ? active.midlight : "#351a42"
            placeholderText: CarboxylAlwaysActive ? active.placeholderText : "#5e5263"
            shadow: CarboxylAlwaysActive ? active.shadow : "#050108"
            text: CarboxylAlwaysActive ? active.text : "#8a7a91"
            toolTipBase: CarboxylAlwaysActive ? active.toolTipBase : "#120a21"
            toolTipText: CarboxylAlwaysActive ? active.toolTipText : "#f5e6ff"
            window: CarboxylAlwaysActive ? active.window : "#1c0a24"
            windowText: CarboxylAlwaysActive ? active.windowText : "#8a7a91"
        }

        disabled {
            alternateBase: "#1c1424"
            base: "#140d1a"
            brightText: "#ffffff"
            button: "#2d2433"
            buttonText: "#5c5061"
            dark: "#0f0214"
            highlightedText: "#5c5061"
            light: "#3d3445"
            link: "#718e9e"
            linkVisited: "#8c7a94"
            mid: "#1a141f"
            midlight: "#3a2f42"
            placeholderText: "#332c38"
            shadow: "#050108"
            text: "#5c5061"
            toolTipBase: "#120a21"
            toolTipText: "#f5e6ff"
            window: "#1a1221"
            windowText: "#5c5061"
        }
    }
}
