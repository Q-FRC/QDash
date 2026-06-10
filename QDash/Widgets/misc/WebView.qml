// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

import QDash.Config
import Carboxyl.Clover

import QtWebEngine

BaseWidget {
    id: widget

    property string item_url: ""
    connected: true

    menuExtension: Component {
        Menu {
            id: webMenu
            title: "Web Actions"

            MenuItem {
                text: "Back"
                onTriggered: web.goBack()
            }

            MenuItem {
                text: "Forward"
                onTriggered: web.goForward()
            }

            MenuItem {
                text: "Reload"
                onTriggered: web.reload()
            }

            MenuItem {
                text: "Sync URL"
                onTriggered: item_url = web.url
            }
        }
    }

    configContent: ColumnLayout {
        id: layout
        spacing: 12
        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true

        SectionHeader {
            label: "Font Settings"
        }

        LabeledSpinBox {
            id: titleFontField
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            label: "Title Font Size"

            bindedProperty: "titleFontSize"
        }

        SectionHeader {
            label: "Web View Settings"
        }

        LabeledTextField {
            id: urlField
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            label: "URL"

            bindedProperty: "item_url"
        }
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        WebEngineView {
            id: web

            anchors.fill: parent
            url: item_url

            // TODO: profiles
            profile: WebEngineProfile {
                storageName: "QDash"
                persistentCookiesPolicy: WebEngineProfile.ForcePersistentCookies
                offTheRecord: false
            }
        }
    }
}
