// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QSignalSpy>
#include <QTest>
#include "Managers/ConnManager.h"
#include "buildconfig/BuildConfig.h"

class TestConnManager : public QObject {
    Q_OBJECT

private slots:
    // ── initial state ──────────────────────────────────────────────────────
    void testInitialAddress()
    {
        ConnManager manager;
        QCOMPARE(manager.address(), "0.0.0.0");
    }

    void testInitialStatus()
    {
        ConnManager manager;
        QCOMPARE(manager.status(), "Not Connected");
    }

    void testInitialTitle()
    {
        ConnManager manager;
        QString expected = BuildConfig.APPLICATION_NAME + " - Not Connected";
        QCOMPARE(manager.title(), expected);
    }

    // ── setConnected ───────────────────────────────────────────────────────
    void testSetConnectedTrueUpdatesConnected()
    {
        ConnManager manager;
        manager.setConnected(false);
        manager.setConnected(true);
        QVERIFY(manager.connected());
    }

    void testSetConnectedFalseUpdatesConnected()
    {
        ConnManager manager;
        manager.setConnected(true);
        manager.setConnected(false);
        QVERIFY(!manager.connected());
    }

    void testSetConnectedTrueUpdatesStatus()
    {
        ConnManager manager;
        manager.setConnected(false);
        manager.setAddress("10.0.0.2");
        manager.setConnected(true);
        QCOMPARE(manager.status(), "Connected (10.0.0.2)");
    }

    void testSetConnectedFalseUpdatesStatus()
    {
        ConnManager manager;
        manager.setConnected(true);
        manager.setConnected(false);
        QCOMPARE(manager.status(), "Not Connected");
    }

    void testSetConnectedTrueUpdatesTitle()
    {
        ConnManager manager;
        manager.setConnected(false);
        manager.setConnected(true);
        QVERIFY(manager.title().contains("Connected"));
        QVERIFY(!manager.title().contains("Not Connected"));
    }

    void testSetConnectedFalseUpdatesTitle()
    {
        ConnManager manager;
        manager.setConnected(false);
        QVERIFY(manager.title().contains("Not Connected"));
    }

    void testConnectedStatusIncludesAddress()
    {
        ConnManager manager;
        manager.setConnected(false);
        manager.setAddress("10.5.0.2");
        manager.setConnected(true);
        QVERIFY(manager.status().contains("10.5.0.2"));
    }

    void testDisconnectedStatusExcludesAddress()
    {
        ConnManager manager;
        manager.setConnected(true);
        manager.setConnected(false);
        QVERIFY(!manager.status().contains("0.0.0.0"));
    }

    // ── setAddress ─────────────────────────────────────────────────────────
    void testSetAddress()
    {
        ConnManager manager;
        manager.setAddress("192.168.1.1");
        QCOMPARE(manager.address(), "192.168.1.1");
    }

    void testSetAddressSameValueEmitsNoSignal()
    {
        ConnManager manager;
        QSignalSpy spy(&manager, &ConnManager::addressChanged);
        manager.setAddress("0.0.0.0");
        QCOMPARE(spy.count(), 0);
    }

    void testSetAddressDifferentValueEmitsSignal()
    {
        ConnManager manager;
        QSignalSpy spy(&manager, &ConnManager::addressChanged);
        manager.setAddress("10.0.0.1");
        QCOMPARE(spy.count(), 1);
    }

    // ── signals ────────────────────────────────────────────────────────────
    void testSetConnectedEmitsConnectedChanged()
    {
        ConnManager manager;
        manager.setConnected(false);
        QSignalSpy spy(&manager, &ConnManager::connectedChanged);
        manager.setConnected(true);
        QCOMPARE(spy.count(), 1);
    }

    void testSetConnectedEmitsStatusChanged()
    {
        ConnManager manager;
        manager.setConnected(false);
        QSignalSpy spy(&manager, &ConnManager::statusChanged);
        manager.setConnected(true);
        QCOMPARE(spy.count(), 1);
    }

    void testSetConnectedEmitsTitleChanged()
    {
        ConnManager manager;
        manager.setConnected(false);
        QSignalSpy spy(&manager, &ConnManager::titleChanged);
        manager.setConnected(true);
        QCOMPARE(spy.count(), 1);
    }

    void testSetConnectedSameValueEmitsNoSignal()
    {
        ConnManager manager;
        manager.setConnected(false);
        QSignalSpy spy(&manager, &ConnManager::connectedChanged);
        manager.setConnected(false);
        QCOMPARE(spy.count(), 0);
    }
};

QTEST_GUILESS_MAIN(TestConnManager)
#include "TestConnManager.moc"
