// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMPILEDEFINITIONS_H
#define COMPILEDEFINITIONS_H

#include <QObject>
#include <QQmlProperty>

#include "Models/GenericMapModel.h"

class CompileDefinitions : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool useWebView READ useWebView FINAL CONSTANT)
    Q_PROPERTY(bool useCameraView READ useCameraView FINAL CONSTANT)
    Q_PROPERTY(bool useNetwork READ useNetwork FINAL CONSTANT)
    Q_PROPERTY(bool tapOnly READ tapOnly FINAL CONSTANT)
    Q_PROPERTY(bool singleFile READ singleFile FINAL CONSTANT)
    Q_PROPERTY(bool brokenMenus READ brokenMenus FINAL CONSTANT)
    Q_PROPERTY(GenericMapModel *extraWidgets READ extraWidgets FINAL CONSTANT)
public:
    using QObject::QObject;

    GenericMapModel *extraWidgets()
    {
        GenericMapModel *model = new GenericMapModel(this);

#ifdef QDASH_WEBVIEW
        model->add("Web View", "web");
#endif
        return model;
    }

    constexpr bool useWebView()
    {
#ifdef QDASH_WEBVIEW
        return true;
#else
        return false;
#endif
    }

    constexpr bool useCameraView()
    {
#ifdef QDASH_CAMVIEW
        return true;
#else
        return false;
#endif
    }

    constexpr bool useNetwork()
    {
#ifdef QDASH_NETWORK
        return true;
#else
        return false;
#endif
    }

    constexpr bool tapOnly() {
#if defined(__ANDROID__) || defined(TARGET_OS_IOS)
        return true;
#else
        return false;
#endif
    }

    constexpr bool singleFile() {
#ifdef QDASH_SINGLEMODE
        return true;
#else
        return false;
#endif
    }

    // Qt for iOS has mildly broken menus
    // You can't call popup()
    constexpr bool brokenMenus() {
#if defined(TARGET_OS_IOS)
        return true;
#else
        return false;
#endif
    }
};

#endif // COMPILEDEFINITIONS_H
