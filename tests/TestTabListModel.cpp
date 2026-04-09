// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>
#include "Logging/Logger.h"
#include "Managers/SettingsManager.h"
#include "Models/TabListModel.h"

class TestTabListModel : public QObject {
    Q_OBJECT

private:
    Logger         *m_logs     = nullptr;
    SettingsManager *m_settings = nullptr;

private slots:
    void initTestCase()
    {
        QCoreApplication::setApplicationName("QDashTests");
        QCoreApplication::setOrganizationName("QFRCTest");
        QStandardPaths::setTestModeEnabled(true);
    }

    void init()
    {
        m_logs     = new Logger;
        m_settings = new SettingsManager(m_logs);
    }

    void cleanup()
    {
        delete m_settings;
        m_settings = nullptr;
        delete m_logs;
        m_logs = nullptr;
    }

    // ── initial state ──────────────────────────────────────────────────────
    void testInitialRowCount()
    {
        TabListModel model(m_logs, m_settings);
        QCOMPARE(model.rowCount(), 0);
    }

    void testInitialSelectedTab()
    {
        TabListModel model(m_logs, m_settings);
        QCOMPARE(model.selectedTab(), 0);
    }

    // ── add(title) ─────────────────────────────────────────────────────────
    void testAddByTitleIncreasesRowCount()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab 1");
        QCOMPARE(model.rowCount(), 1);
    }

    void testAddByTitleSetsTitle()
    {
        TabListModel model(m_logs, m_settings);
        model.add("My Tab");
        QCOMPARE(model.data(model.index(0), TabListModel::TITLE).toString(), "My Tab");
    }

    void testAddByTitleDefaultRows()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab 1");
        QCOMPARE(model.data(model.index(0), TabListModel::ROWS).toInt(), 3);
    }

    void testAddByTitleDefaultCols()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab 1");
        QCOMPARE(model.data(model.index(0), TabListModel::COLS).toInt(), 5);
    }

    void testAddByTitleCreatesWidgetsModel()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab 1");
        QVariant widgets = model.data(model.index(0), TabListModel::WIDGETS);
        QVERIFY(!widgets.isNull());
        QVERIFY(widgets.value<TabWidgetsModel *>() != nullptr);
    }

    void testAddByTitleMultipleTabs()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        model.add("B");
        model.add("C");
        QCOMPARE(model.rowCount(), 3);
    }

    // ── add(Tab) ───────────────────────────────────────────────────────────
    void testAddTabStruct()
    {
        TabListModel model(m_logs, m_settings);
        Tab t;
        t.title = "Custom";
        t.rows  = 6;
        t.cols  = 8;
        t.model = new TabWidgetsModel(&model);
        t.model->setRows(t.rows);
        t.model->setCols(t.cols);
        model.add(t);

        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.data(model.index(0), TabListModel::TITLE).toString(), "Custom");
        QCOMPARE(model.data(model.index(0), TabListModel::ROWS).toInt(), 6);
        QCOMPARE(model.data(model.index(0), TabListModel::COLS).toInt(), 8);
    }

    // ── remove ─────────────────────────────────────────────────────────────
    void testRemoveDecreasesRowCount()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        model.add("B");
        model.remove(0);
        QCOMPARE(model.rowCount(), 1);
    }

    void testRemoveShiftsRemainingItems()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        model.add("B");
        model.remove(0);
        QCOMPARE(model.data(model.index(0), TabListModel::TITLE).toString(), "B");
    }

    void testRemoveLast()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        model.add("B");
        model.remove(1);
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.data(model.index(0), TabListModel::TITLE).toString(), "A");
    }

    // ── setData ────────────────────────────────────────────────────────────
    void testSetDataTitle()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Old");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, "New", TabListModel::TITLE));
        QCOMPARE(model.data(idx, TabListModel::TITLE).toString(), "New");
    }

    void testSetDataRows()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, 6, TabListModel::ROWS));
        QCOMPARE(model.data(idx, TabListModel::ROWS).toInt(), 6);
    }

    void testSetDataCols()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, 8, TabListModel::COLS));
        QCOMPARE(model.data(idx, TabListModel::COLS).toInt(), 8);
    }

    void testSetDataNoOpWhenSameValue()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab");
        QModelIndex idx = model.index(0);
        QVERIFY(!model.setData(idx, "Tab", TabListModel::TITLE));
    }

    void testSetDataEmitsDataChanged()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Old");
        QSignalSpy spy(&model, &TabListModel::dataChanged);
        model.setData(model.index(0), "New", TabListModel::TITLE);
        QCOMPARE(spy.count(), 1);
    }

    // ── clear ──────────────────────────────────────────────────────────────
    void testClearEmptiesModel()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        model.add("B");
        model.clear();
        QCOMPARE(model.rowCount(), 0);
    }

    void testClearOnEmptyModelIsNoOp()
    {
        TabListModel model(m_logs, m_settings);
        model.clear();
        QCOMPARE(model.rowCount(), 0);
    }

    // ── selectTab ──────────────────────────────────────────────────────────
    void testSelectTabByName()
    {
        TabListModel model(m_logs, m_settings);
        model.add("First");
        model.add("Second");
        model.add("Third");
        model.selectTab("Third");
        QCOMPARE(model.selectedTab(), 2);
    }

    void testSelectTabFirstEntry()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Alpha");
        model.add("Beta");
        model.selectTab("Alpha");
        QCOMPARE(model.selectedTab(), 0);
    }

    void testSelectTabNonexistentDoesNotCrash()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        // Selecting a tab that doesn't exist should not crash; selectedTab stays unchanged
        int before = model.selectedTab();
        model.selectTab("DoesNotExist");
        QCOMPARE(model.selectedTab(), before);
    }

    void testSelectTabEmitsSignal()
    {
        TabListModel model(m_logs, m_settings);
        model.add("A");
        model.add("B");
        QSignalSpy spy(&model, &TabListModel::selectedTabChanged);
        model.selectTab("B");
        QCOMPARE(spy.count(), 1);
    }

    // ── saveObject (JSON) ──────────────────────────────────────────────────
    void testSaveObjectIsValidJson()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab1");
        QJsonDocument doc = model.saveObject();
        QVERIFY(!doc.isNull());
        QVERIFY(doc.isObject());
    }

    void testSaveObjectContainsTabsArray()
    {
        TabListModel model(m_logs, m_settings);
        model.add("Tab1");
        QJsonObject obj = model.saveObject().object();
        QVERIFY(obj.contains("tabs"));
        QVERIFY(obj["tabs"].isArray());
        QCOMPARE(obj["tabs"].toArray().size(), 1);
    }

    void testSaveObjectTabTitle()
    {
        TabListModel model(m_logs, m_settings);
        model.add("MyTab");
        QJsonObject tab = model.saveObject().object()["tabs"].toArray()[0].toObject();
        QCOMPARE(tab["title"].toString(), "MyTab");
    }

    void testSaveObjectTabRowsCols()
    {
        TabListModel model(m_logs, m_settings);
        model.add("T");
        QJsonObject tab = model.saveObject().object()["tabs"].toArray()[0].toObject();
        QCOMPARE(tab["rows"].toInt(), 3);
        QCOMPARE(tab["cols"].toInt(), 5);
    }

    void testSaveObjectEmptyTabsArray()
    {
        TabListModel model(m_logs, m_settings);
        QJsonObject obj = model.saveObject().object();
        QCOMPARE(obj["tabs"].toArray().size(), 0);
    }

    void testSaveObjectContainsConnectionFields()
    {
        TabListModel model(m_logs, m_settings);
        QJsonObject obj = model.saveObject().object();
        QVERIFY(obj.contains("mode"));
        QVERIFY(obj.contains("team"));
        QVERIFY(obj.contains("ip"));
    }

    // ── flags ──────────────────────────────────────────────────────────────
    void testFlagsValidIndex()
    {
        TabListModel model(m_logs, m_settings);
        model.add("T");
        QVERIFY(model.flags(model.index(0)) & Qt::ItemIsEditable);
    }

    void testFlagsInvalidIndex()
    {
        TabListModel model(m_logs, m_settings);
        QVERIFY(!(model.flags(QModelIndex()) & Qt::ItemIsEditable));
    }

    // ── roleNames ──────────────────────────────────────────────────────────
    void testRoleNames()
    {
        TabListModel model(m_logs, m_settings);
        QHash<int, QByteArray> roles = model.roleNames();
        QCOMPARE(roles[TabListModel::TITLE],   QByteArray("title"));
        QCOMPARE(roles[TabListModel::ROWS],    QByteArray("rows"));
        QCOMPARE(roles[TabListModel::COLS],    QByteArray("cols"));
        QCOMPARE(roles[TabListModel::WIDGETS], QByteArray("widgets"));
    }
};

QTEST_GUILESS_MAIN(TestTabListModel)
#include "TestTabListModel.moc"
