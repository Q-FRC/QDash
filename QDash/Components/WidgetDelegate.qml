// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QDash.Widgets
import Qt.labs.qmlmodels
import QtQml.Models
import QtQuick 6.8

DelegateChooser {
    DelegateChooser {
        id: chooser

        role: "type"

        DelegateChoice {
            roleValue: "int"

            IntWidget {}
        }

        DelegateChoice {
            roleValue: "string"

            TextWidget {}
        }

        DelegateChoice {
            roleValue: "double"

            DoubleWidget {}
        }

        DelegateChoice {
            roleValue: "bool"

            BoolWidget {}
        }

        DelegateChoice {
            roleValue: "dial"

            IntDialWidget {}
        }

        DelegateChoice {
            roleValue: "doubleDial"

            DoubleDialWidget {}
        }

        DelegateChoice {
            roleValue: "color"

            ColorWidget {}
        }

        DelegateChoice {
            roleValue: "FMSInfo"

            FMSInfo {}
        }

        DelegateChoice {
            roleValue: "Field2d"

            Field2d {}
        }

        DelegateChoice {
            roleValue: "Command"

            Command {}
        }

        DelegateChoice {
            roleValue: "String Chooser"

            StringChooser {}
        }

        DelegateChoice {
            roleValue: "camera"

            // TODO: generify
            delegate: Loader {
                sourceComponent: CompileDefinitions.useCameraView ? Qt.createComponent("../Widgets/Misc/CameraView.qml") : null
                z: 3
            }
        }

        DelegateChoice {
            roleValue: "web"

            delegate: Loader {
                sourceComponent: CompileDefinitions.useWebView ? Qt.createComponent("../Widgets/Misc/WebView.qml") : null
                z: 3
            }
        }

        DelegateChoice {
            roleValue: "urlCamera"

            delegate: Loader {
                sourceComponent: CompileDefinitions.useCameraView ? Qt.createComponent("../Widgets/Misc/UrlCameraView.qml") : null
                z: 3
            }
        }

        DelegateChoice {
            roleValue: "colorText"

            ColorTextWidget {}
        }

        DelegateChoice {
            roleValue: "doubleGauge"

            DoubleGaugeWidget {}
        }

        DelegateChoice {
            roleValue: "doubleBar"

            DoubleProgressBar {}
        }

        DelegateChoice {
            roleValue: "doubleDisplay"

            DoubleDisplay {}
        }

        DelegateChoice {
            roleValue: "intDisplay"

            IntDisplay {}
        }

        DelegateChoice {
            roleValue: "matchTime"

            MatchTime {}
        }

        DelegateChoice {
            roleValue: "textDisplay"

            StringDisplay {}
        }
    }

    Component {
        id: nullComponent

        Item {}
    }
}
