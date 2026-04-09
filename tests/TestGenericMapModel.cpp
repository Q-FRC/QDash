// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>
#include "Models/GenericMapModel.h"

class TestGenericMapModel : public QObject {
    Q_OBJECT

private slots:
    // ── initial state ──────────────────────────────────────────────────────
    void testInitialState()
    {
        GenericMapModel model;
        QCOMPARE(model.rowCount(), 0);
        QCOMPARE(model.count(), 0);
    }

    // ── add ────────────────────────────────────────────────────────────────
    void testAddIncreasesRowCount()
    {
        GenericMapModel model;
        model.add("key1", "value1");
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.count(), 1);
    }

    void testAddMultipleEntries()
    {
        GenericMapModel model;
        model.add("a", "1");
        model.add("b", "2");
        model.add("c", "3");
        QCOMPARE(model.rowCount(), 3);
        QCOMPARE(model.count(), 3);
    }

    void testAddPreservesOrder()
    {
        GenericMapModel model;
        model.add("first", "1");
        model.add("second", "2");
        QCOMPARE(model.data(model.index(0), GenericMapModel::KEY).toString(), "first");
        QCOMPARE(model.data(model.index(1), GenericMapModel::KEY).toString(), "second");
    }

    // ── data roles ─────────────────────────────────────────────────────────
    void testDataKeyRole()
    {
        GenericMapModel model;
        model.add("myKey", "myValue");
        QCOMPARE(model.data(model.index(0), GenericMapModel::KEY).toString(), "myKey");
    }

    void testDataValueRole()
    {
        GenericMapModel model;
        model.add("myKey", "myValue");
        QCOMPARE(model.data(model.index(0), GenericMapModel::VALUE).toString(), "myValue");
    }

    void testDataSecondEntry()
    {
        GenericMapModel model;
        model.add("a", "alpha");
        model.add("b", "beta");
        QCOMPARE(model.data(model.index(1), GenericMapModel::KEY).toString(), "b");
        QCOMPARE(model.data(model.index(1), GenericMapModel::VALUE).toString(), "beta");
    }

    void testDataInvalidIndexReturnsNull()
    {
        GenericMapModel model;
        QCOMPARE(model.data(QModelIndex(), GenericMapModel::KEY), QVariant());
    }

    void testDataUnknownRoleReturnsNull()
    {
        GenericMapModel model;
        model.add("k", "v");
        QCOMPARE(model.data(model.index(0), Qt::DecorationRole), QVariant());
    }

    void testDataEmptyStringValues()
    {
        GenericMapModel model;
        model.add("", "");
        QCOMPARE(model.data(model.index(0), GenericMapModel::KEY).toString(), "");
        QCOMPARE(model.data(model.index(0), GenericMapModel::VALUE).toString(), "");
    }

    // ── count ──────────────────────────────────────────────────────────────
    void testCountMatchesRowCount()
    {
        GenericMapModel model;
        model.add("a", "1");
        model.add("b", "2");
        QCOMPARE(model.count(), model.rowCount());
    }

    // ── roleNames ──────────────────────────────────────────────────────────
    void testRoleNames()
    {
        GenericMapModel model;
        QHash<int, QByteArray> roles = model.roleNames();
        QCOMPARE(roles[GenericMapModel::KEY],   QByteArray("key"));
        QCOMPARE(roles[GenericMapModel::VALUE], QByteArray("value"));
    }
};

QTEST_GUILESS_MAIN(TestGenericMapModel)
#include "TestGenericMapModel.moc"
