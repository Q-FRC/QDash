// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

import QtQuick

QtObject {
    function searchFunction(node, funcName) {
        if (!node || !node.children)
            return null

        for (var i = 0; i < node.children.length; ++i) {
            var child = node.children[i]
            if (!child)
                continue
            if (funcName in child) {
                child[funcName]()
            } else {
                searchFunction(child, funcName)
            }
        }

        return null
    }

    function typeName(obj) {
        const name = obj.toString();

        // remove address suffix
        const cleaned = name.split("(")[0];

        // remove QQuick prefix
        if (cleaned.includes("QQuick"))
            return cleaned.replace("QQuick", "")

        // remove QMLTYPE suffix
        if (cleaned.includes("QMLTYPE"))
            return cleaned.split("_QMLTYPE_")[0]
    }
}
