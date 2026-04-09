// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QSettings>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>
#include "Logging/Logger.h"
#include "Managers/SettingsManager.h"
#include "Misc/Constants.h"

class TestSettingsManager : public QObject {
    Q_OBJECT

private:
    Logger          *m_logs     = nullptr;
    SettingsManager *m_settings = nullptr;

    void clearSettings()
    {
        QSettings s(qApp);
        s.remove(Settings::RecentFiles.Name);
    }

private slots:
    void initTestCase()
    {
        QCoreApplication::setApplicationName("QDashTestsSettings");
        QCoreApplication::setOrganizationName("QFRCTestSettings");
        QStandardPaths::setTestModeEnabled(true);
    }

    void init()
    {
        clearSettings();
        m_logs     = new Logger;
        m_settings = new SettingsManager(m_logs);
    }

    void cleanup()
    {
        delete m_settings;
        m_settings = nullptr;
        delete m_logs;
        m_logs = nullptr;
        clearSettings();
    }

    // ── addRecentFile(QString) ─────────────────────────────────────────────
    void testAddRecentFilePrependsNewEntry()
    {
        m_settings->addRecentFile("/path/to/file.qdash");
        QStringList recent = m_settings->recentFiles();
        QVERIFY(!recent.isEmpty());
        QCOMPARE(recent.first(), "/path/to/file.qdash");
    }

    void testAddRecentFilePrependsSecondEntry()
    {
        m_settings->addRecentFile("/first.qdash");
        m_settings->addRecentFile("/second.qdash");
        QStringList recent = m_settings->recentFiles();
        QCOMPARE(recent[0], "/second.qdash");
        QCOMPARE(recent[1], "/first.qdash");
    }

    void testAddRecentFileDuplicateMovesToFront()
    {
        m_settings->addRecentFile("/a.qdash");
        m_settings->addRecentFile("/b.qdash");
        m_settings->addRecentFile("/c.qdash");
        // Adding /a.qdash again should move it to front, not duplicate it
        m_settings->addRecentFile("/a.qdash");
        QStringList recent = m_settings->recentFiles();
        QCOMPARE(recent.first(), "/a.qdash");
        QCOMPARE(recent.count("/a.qdash"), 1);
    }

    void testAddRecentFileDuplicateDoesNotIncreaseSize()
    {
        m_settings->addRecentFile("/a.qdash");
        m_settings->addRecentFile("/b.qdash");
        int sizeBefore = m_settings->recentFiles().size();
        m_settings->addRecentFile("/a.qdash");
        QCOMPARE(m_settings->recentFiles().size(), sizeBefore);
    }

    void testAddRecentFileCapsAtFive()
    {
        for (int i = 0; i < 6; ++i)
            m_settings->addRecentFile(QString("/file%1.qdash").arg(i));
        QVERIFY(m_settings->recentFiles().size() <= 5);
    }

    void testAddRecentFileCapsDropsOldest()
    {
        // Add 5 files first
        for (int i = 0; i < 5; ++i)
            m_settings->addRecentFile(QString("/file%1.qdash").arg(i));
        // The 6th file should bump the oldest (file0) off the list
        m_settings->addRecentFile("/newest.qdash");
        QStringList recent = m_settings->recentFiles();
        QVERIFY(!recent.contains("/file0.qdash"));
        QVERIFY(recent.contains("/newest.qdash"));
    }

    void testAddRecentFileMostRecentIsFirst()
    {
        m_settings->addRecentFile("/old.qdash");
        m_settings->addRecentFile("/new.qdash");
        QCOMPARE(m_settings->recentFiles().first(), "/new.qdash");
    }

    // ── signal emission ────────────────────────────────────────────────────
    void testAddRecentFileEmitsSignal()
    {
        QSignalSpy spy(m_settings, &SettingsManager::recentFilesChanged);
        m_settings->addRecentFile("/some.qdash");
        QCOMPARE(spy.count(), 1);
    }

    void testAddRecentFileDuplicateEmitsSignal()
    {
        m_settings->addRecentFile("/a.qdash");
        QSignalSpy spy(m_settings, &SettingsManager::recentFilesChanged);
        m_settings->addRecentFile("/a.qdash");
        QCOMPARE(spy.count(), 1);
    }

    // ── persistence via QSettings ──────────────────────────────────────────
    void testRecentFilesArePersisted()
    {
        m_settings->addRecentFile("/persisted.qdash");
        // Create a fresh SettingsManager reading the same QSettings
        Logger fresh_logs;
        SettingsManager fresh(&fresh_logs);
        QVERIFY(fresh.recentFiles().contains("/persisted.qdash"));
    }
};

QTEST_GUILESS_MAIN(TestSettingsManager)
#include "TestSettingsManager.moc"
