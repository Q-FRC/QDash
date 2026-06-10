// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import Carboxyl.Clover

import QDash.Config
import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

import QtWebEngine

BaseWidget {
    id: widget

    readOnly: false
    roleString: "web"

    property string item_url: ""

    connected: true

    configComponent: Component {
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 2
            clip: true
            spacing: 12

            SectionHeader {
                label: "Font Settings"
            }

            LabeledSpinBox {
                id: titleFontField

                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                bindedProperty: "titleFontSize"
                label: "Title Font Size"
            }

            SectionHeader {
                label: "Web View Settings"
            }

            LabeledTextField {
                id: urlField

                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                bindedProperty: "item_url"
                label: "URL"
            }
        }
    }
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

    Item {
        anchors {
            bottom: parent.bottom
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
            top: titleField.bottom
        }

        WebEngineView {
            id: web

            anchors.fill: parent
            url: item_url

            // TODO: profiles
            profile: WebEngineProfile {
                offTheRecord: false
                persistentCookiesPolicy: WebEngineProfile.ForcePersistentCookies
                storageName: "QDash"
            }
        }
    }
}
