// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtQuickTest>
#include <QQmlEngine>

/**
 * Setup object passed to QUICK_TEST_MAIN_WITH_SETUP.
 *
 * The qmlEngineAvailable() slot runs before the first QML test file is
 * loaded, giving us a chance to add the project's QML output directory to
 * the import path so that "import QDash.Items" / "import QDash.Config" etc.
 * resolve correctly in the test QML files.
 */
class QDashQuickTestSetup : public QObject
{
    Q_OBJECT

public:
    explicit QDashQuickTestSetup() = default;

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        // QDASH_QML_PATH is injected at compile time by CMakeLists.txt as
        // the path to ${CMAKE_BINARY_DIR}/qml where all QML modules live.
        engine->addImportPath(QStringLiteral(QDASH_QML_PATH));
    }
};

QUICK_TEST_MAIN_WITH_SETUP(QmlComponents, QDashQuickTestSetup)

#include "tst_QmlComponents.moc"
