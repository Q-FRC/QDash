// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QJsonArray>
#include <QSignalSpy>
#include <QTest>
#include "Models/TabWidgetsModel.h"

class TestTabWidgetsModel : public QObject {
    Q_OBJECT

private:
    static Widget makeWidget(const QString &title, const QString &topic, const QString &type,
                             int row, int col, int rowSpan = 1, int colSpan = 1)
    {
        Widget w;
        w.title   = title;
        w.topic   = topic;
        w.type    = type;
        w.row     = row;
        w.col     = col;
        w.rowSpan = rowSpan;
        w.colSpan = colSpan;
        return w;
    }

private slots:
    // ── initial state ──────────────────────────────────────────────────────
    void testInitialState()
    {
        TabWidgetsModel model;
        QCOMPARE(model.rowCount(), 0);
    }

    // ── add ────────────────────────────────────────────────────────────────
    void testAddByTitleTopicType()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QCOMPARE(model.rowCount(), 1);
    }

    void testAddStructWidget()
    {
        TabWidgetsModel model;
        model.add(makeWidget("Test", "/test", "number", 1, 2));
        QCOMPARE(model.rowCount(), 1);
    }

    void testAddDefaultsRowColToMinusOne()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QCOMPARE(model.data(idx, TabWidgetsModel::ROW).toInt(), -1);
        QCOMPARE(model.data(idx, TabWidgetsModel::COL).toInt(), -1);
    }

    void testAddDefaultsSpanToOne()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QCOMPARE(model.data(idx, TabWidgetsModel::ROWSPAN).toInt(), 1);
        QCOMPARE(model.data(idx, TabWidgetsModel::COLSPAN).toInt(), 1);
    }

    void testAddMultipleWidgets()
    {
        TabWidgetsModel model;
        for (int i = 0; i < 5; ++i)
            model.add(QString("W%1").arg(i), QString("/t%1").arg(i), "number");
        QCOMPARE(model.rowCount(), 5);
    }

    // ── data roles ─────────────────────────────────────────────────────────
    void testDataTitle()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TITLE).toString(), "Speed");
    }

    void testDataTopic()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TOPIC).toString(), "/speed");
    }

    void testDataType()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TYPE).toString(), "number");
    }

    void testDataRowAndCol()
    {
        TabWidgetsModel model;
        model.add(makeWidget("W", "/w", "bool", 2, 3));
        QModelIndex idx = model.index(0);
        QCOMPARE(model.data(idx, TabWidgetsModel::ROW).toInt(), 2);
        QCOMPARE(model.data(idx, TabWidgetsModel::COL).toInt(), 3);
    }

    void testDataRowSpanAndColSpan()
    {
        TabWidgetsModel model;
        model.add(makeWidget("W", "/w", "bool", 0, 0, 2, 3));
        QModelIndex idx = model.index(0);
        QCOMPARE(model.data(idx, TabWidgetsModel::ROWSPAN).toInt(), 2);
        QCOMPARE(model.data(idx, TabWidgetsModel::COLSPAN).toInt(), 3);
    }

    void testDataProperties()
    {
        TabWidgetsModel model;
        Widget w = makeWidget("W", "/w", "number", 0, 0);
        w.properties["min"] = 0;
        w.properties["max"] = 100;
        model.add(w);
        QVariantMap props = model.data(model.index(0), TabWidgetsModel::PROPERTIES).toMap();
        QCOMPARE(props["min"].toInt(), 0);
        QCOMPARE(props["max"].toInt(), 100);
    }

    void testDataIndex()
    {
        TabWidgetsModel model;
        model.add("A", "/a", "number");
        model.add("B", "/b", "number");
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::IDX).toInt(), 0);
        QCOMPARE(model.data(model.index(1), TabWidgetsModel::IDX).toInt(), 1);
    }

    void testDataInvalidIndexReturnsNull()
    {
        TabWidgetsModel model;
        QCOMPARE(model.data(QModelIndex(), TabWidgetsModel::TITLE), QVariant());
    }

    void testDataUnknownRoleReturnsNull()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QCOMPARE(model.data(model.index(0), Qt::DecorationRole), QVariant());
    }

    // ── setData ────────────────────────────────────────────────────────────
    void testSetDataTitle()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, "Velocity", TabWidgetsModel::TITLE));
        QCOMPARE(model.data(idx, TabWidgetsModel::TITLE).toString(), "Velocity");
    }

    void testSetDataTopic()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, "/velocity", TabWidgetsModel::TOPIC));
        QCOMPARE(model.data(idx, TabWidgetsModel::TOPIC).toString(), "/velocity");
    }

    void testSetDataType()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, "gauge", TabWidgetsModel::TYPE));
        QCOMPARE(model.data(idx, TabWidgetsModel::TYPE).toString(), "gauge");
    }

    void testSetDataRow()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, 2, TabWidgetsModel::ROW));
        QCOMPARE(model.data(idx, TabWidgetsModel::ROW).toInt(), 2);
    }

    void testSetDataCol()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, 3, TabWidgetsModel::COL));
        QCOMPARE(model.data(idx, TabWidgetsModel::COL).toInt(), 3);
    }

    void testSetDataRowSpan()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, 2, TabWidgetsModel::ROWSPAN));
        QCOMPARE(model.data(idx, TabWidgetsModel::ROWSPAN).toInt(), 2);
    }

    void testSetDataColSpan()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(model.setData(idx, 2, TabWidgetsModel::COLSPAN));
        QCOMPARE(model.data(idx, TabWidgetsModel::COLSPAN).toInt(), 2);
    }

    void testSetDataProperties()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVariantMap props;
        props["min"] = 0;
        props["max"] = 100;
        QVERIFY(model.setData(idx, props, TabWidgetsModel::PROPERTIES));
        QCOMPARE(model.data(idx, TabWidgetsModel::PROPERTIES).toMap(), props);
    }

    void testSetDataNoOpWhenSameValue()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QModelIndex idx = model.index(0);
        QVERIFY(!model.setData(idx, "Speed", TabWidgetsModel::TITLE));
    }

    void testSetDataEmitsDataChanged()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QSignalSpy spy(&model, &TabWidgetsModel::dataChanged);
        model.setData(model.index(0), "Velocity", TabWidgetsModel::TITLE);
        QCOMPARE(spy.count(), 1);
    }

    // ── remove ─────────────────────────────────────────────────────────────
    void testRemoveByIndex()
    {
        TabWidgetsModel model;
        model.add("A", "/a", "number");
        model.add("B", "/b", "number");
        QVERIFY(model.remove(0));
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TITLE).toString(), "B");
    }

    void testRemoveLastByIndex()
    {
        TabWidgetsModel model;
        model.add("A", "/a", "number");
        model.add("B", "/b", "number");
        QVERIFY(model.remove(1));
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TITLE).toString(), "A");
    }

    void testRemoveNegativeIndexReturnsFalse()
    {
        TabWidgetsModel model;
        QVERIFY(!model.remove(-1));
    }

    void testRemoveOutOfBoundsReturnsFalse()
    {
        TabWidgetsModel model;
        QVERIFY(!model.remove(0));
        QVERIFY(!model.remove(99));
    }

    void testRemoveLatest()
    {
        TabWidgetsModel model;
        model.add("A", "/a", "number");
        model.add("B", "/b", "number");
        QVERIFY(model.removeLatest());
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TITLE).toString(), "A");
    }

    // ── copy ───────────────────────────────────────────────────────────────
    void testCopyPreservesMetadata()
    {
        TabWidgetsModel model;
        model.add(makeWidget("Speed", "/speed", "number", 2, 3, 2, 2));
        Widget copy = model.copy(0);
        QCOMPARE(copy.title, "Speed");
        QCOMPARE(copy.topic, "/speed");
        QCOMPARE(copy.type, "number");
    }

    void testCopyResetsPosition()
    {
        TabWidgetsModel model;
        model.add(makeWidget("Speed", "/speed", "number", 2, 3, 2, 2));
        Widget copy = model.copy(0);
        QCOMPARE(copy.row, -1);
        QCOMPARE(copy.col, -1);
        QCOMPARE(copy.rowSpan, 1);
        QCOMPARE(copy.colSpan, 1);
    }

    // ── reset ──────────────────────────────────────────────────────────────
    void testReset()
    {
        TabWidgetsModel model;
        model.add("Old", "/old", "number");
        QList<Widget> widgets = {makeWidget("New", "/new", "bool", 0, 0)};
        model.reset(widgets);
        QCOMPARE(model.rowCount(), 1);
        QCOMPARE(model.data(model.index(0), TabWidgetsModel::TITLE).toString(), "New");
    }

    void testResetClearsOldData()
    {
        TabWidgetsModel model;
        model.add("A", "/a", "number");
        model.add("B", "/b", "number");
        model.reset({});
        QCOMPARE(model.rowCount(), 0);
    }

    // ── setEqualTo ─────────────────────────────────────────────────────────
    void testSetEqualTo()
    {
        TabWidgetsModel source;
        source.setRows(4);
        source.setCols(6);
        source.add("Speed", "/speed", "number");
        source.add("Voltage", "/voltage", "bool");

        TabWidgetsModel dest;
        dest.setEqualTo(&source);

        QCOMPARE(dest.rowCount(), 2);
        QCOMPARE(dest.rows(), 4);
        QCOMPARE(dest.cols(), 6);
        QCOMPARE(dest.data(dest.index(0), TabWidgetsModel::TITLE).toString(), "Speed");
        QCOMPARE(dest.data(dest.index(1), TabWidgetsModel::TITLE).toString(), "Voltage");
    }

    void testSetEqualToNullIsNoOp()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        model.setEqualTo(nullptr);
        QCOMPARE(model.rowCount(), 1);
    }

    void testSetEqualToSelfIsNoOp()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        model.setEqualTo(&model);
        QCOMPARE(model.rowCount(), 1);
    }

    // ── rows / cols ────────────────────────────────────────────────────────
    void testSetRows()
    {
        TabWidgetsModel model;
        model.setRows(5);
        QCOMPARE(model.rows(), 5);
    }

    void testSetCols()
    {
        TabWidgetsModel model;
        model.setCols(3);
        QCOMPARE(model.cols(), 3);
    }

    void testSetRowsEmitsSignal()
    {
        TabWidgetsModel model;
        QSignalSpy spy(&model, &TabWidgetsModel::rowsChanged);
        model.setRows(5);
        QCOMPARE(spy.count(), 1);
    }

    void testSetColsEmitsSignal()
    {
        TabWidgetsModel model;
        QSignalSpy spy(&model, &TabWidgetsModel::colsChanged);
        model.setCols(3);
        QCOMPARE(spy.count(), 1);
    }

    void testSetRowsSameValueNoSignal()
    {
        TabWidgetsModel model;
        model.setRows(5);
        QSignalSpy spy(&model, &TabWidgetsModel::rowsChanged);
        model.setRows(5);
        QCOMPARE(spy.count(), 0);
    }

    void testSetColsSameValueNoSignal()
    {
        TabWidgetsModel model;
        model.setCols(3);
        QSignalSpy spy(&model, &TabWidgetsModel::colsChanged);
        model.setCols(3);
        QCOMPARE(spy.count(), 0);
    }

    // ── cellOccupied ───────────────────────────────────────────────────────
    void testCellOccupiedEmptyGrid()
    {
        TabWidgetsModel model;
        model.setRows(5);
        model.setCols(5);
        QVERIFY(!model.cellOccupied(0, 0));
    }

    void testCellOccupiedByWidget()
    {
        TabWidgetsModel model;
        model.setRows(5);
        model.setCols(5);
        model.add(makeWidget("W", "/w", "number", 1, 2));
        QVERIFY(model.cellOccupied(1, 2));
    }

    void testCellNotOccupied()
    {
        TabWidgetsModel model;
        model.setRows(5);
        model.setCols(5);
        model.add(makeWidget("W", "/w", "number", 1, 2));
        QVERIFY(!model.cellOccupied(0, 0));
        QVERIFY(!model.cellOccupied(2, 3));
    }

    void testCellOccupiedOutOfBoundsRow()
    {
        TabWidgetsModel model;
        model.setRows(3);
        model.setCols(3);
        QVERIFY(model.cellOccupied(3, 0));
    }

    void testCellOccupiedOutOfBoundsCol()
    {
        TabWidgetsModel model;
        model.setRows(3);
        model.setCols(3);
        QVERIFY(model.cellOccupied(0, 3));
    }

    void testCellOccupiedSpanExceedsBounds()
    {
        TabWidgetsModel model;
        model.setRows(3);
        model.setCols(3);
        QVERIFY(model.cellOccupied(2, 2, 2, 2));
    }

    void testCellOccupiedMultiSpanWidget()
    {
        TabWidgetsModel model;
        model.setRows(5);
        model.setCols(5);
        // 2×2 widget at (1,1) covers (1,1),(1,2),(2,1),(2,2)
        model.add(makeWidget("W", "/w", "number", 1, 1, 2, 2));
        QVERIFY(model.cellOccupied(1, 2));
        QVERIFY(model.cellOccupied(2, 1));
        QVERIFY(!model.cellOccupied(0, 0));
        QVERIFY(!model.cellOccupied(3, 3));
    }

    void testCellOccupiedWithIgnoreRect()
    {
        TabWidgetsModel model;
        model.setRows(5);
        model.setCols(5);
        model.add(makeWidget("W", "/w", "number", 1, 1));
        // Ignore the widget at (1,1) – cell should not be occupied
        QVERIFY(!model.cellOccupied(1, 1, 1, 1, QRectF(1, 1, 1, 1)));
    }

    // ── save / load round-trip ─────────────────────────────────────────────
    void testSaveObjectEmpty()
    {
        TabWidgetsModel model;
        QCOMPARE(model.saveObject().size(), 0);
    }

    void testSaveObjectContainsExpectedFields()
    {
        TabWidgetsModel model;
        Widget w = makeWidget("Speed", "/speed", "number", 0, 0, 1, 2);
        w.properties["min"] = 0;
        model.add(w);

        QJsonArray arr = model.saveObject();
        QCOMPARE(arr.size(), 1);

        QJsonObject obj = arr[0].toObject();
        QCOMPARE(obj["title"].toString(), "Speed");
        QCOMPARE(obj["topic"].toString(), "/speed");
        QCOMPARE(obj["type"].toString(), "number");
        QCOMPARE(obj["column"].toInt(), 0);
        QCOMPARE(obj["row"].toInt(), 0);
        QCOMPARE(obj["rowSpan"].toInt(), 1);
        QCOMPARE(obj["colSpan"].toInt(), 2);
        QCOMPARE(obj["properties"].toObject()["min"].toInt(), 0);
    }

    void testLoadObjectRoundTrip()
    {
        TabWidgetsModel source;
        Widget w = makeWidget("Speed", "/speed", "number", 0, 0, 1, 2);
        w.properties["max"] = 100;
        source.add(w);

        QJsonArray arr = source.saveObject();
        TabWidgetsModel *loaded = TabWidgetsModel::loadObject(nullptr, arr);

        QCOMPARE(loaded->rowCount(), 1);
        QModelIndex idx = loaded->index(0);
        QCOMPARE(loaded->data(idx, TabWidgetsModel::TITLE).toString(), "Speed");
        QCOMPARE(loaded->data(idx, TabWidgetsModel::TOPIC).toString(), "/speed");
        QCOMPARE(loaded->data(idx, TabWidgetsModel::TYPE).toString(), "number");
        QCOMPARE(loaded->data(idx, TabWidgetsModel::ROW).toInt(), 0);
        QCOMPARE(loaded->data(idx, TabWidgetsModel::COL).toInt(), 0);
        QCOMPARE(loaded->data(idx, TabWidgetsModel::ROWSPAN).toInt(), 1);
        QCOMPARE(loaded->data(idx, TabWidgetsModel::COLSPAN).toInt(), 2);
        QCOMPARE(loaded->data(idx, TabWidgetsModel::PROPERTIES).toMap()["max"].toInt(), 100);

        delete loaded;
    }

    void testLoadObjectEmptyArray()
    {
        QJsonArray arr;
        TabWidgetsModel *loaded = TabWidgetsModel::loadObject(nullptr, arr);
        QCOMPARE(loaded->rowCount(), 0);
        delete loaded;
    }

    void testLoadObjectMultipleWidgets()
    {
        TabWidgetsModel source;
        source.add("A", "/a", "number");
        source.add("B", "/b", "bool");

        TabWidgetsModel *loaded = TabWidgetsModel::loadObject(nullptr, source.saveObject());
        QCOMPARE(loaded->rowCount(), 2);
        QCOMPARE(loaded->data(loaded->index(0), TabWidgetsModel::TITLE).toString(), "A");
        QCOMPARE(loaded->data(loaded->index(1), TabWidgetsModel::TITLE).toString(), "B");
        delete loaded;
    }

    // ── flags ──────────────────────────────────────────────────────────────
    void testFlagsValidIndex()
    {
        TabWidgetsModel model;
        model.add("Speed", "/speed", "number");
        QVERIFY(model.flags(model.index(0)) & Qt::ItemIsEditable);
    }

    void testFlagsInvalidIndex()
    {
        TabWidgetsModel model;
        QVERIFY(!(model.flags(QModelIndex()) & Qt::ItemIsEditable));
    }

    // ── roleNames ──────────────────────────────────────────────────────────
    void testRoleNames()
    {
        TabWidgetsModel model;
        QHash<int, QByteArray> roles = model.roleNames();
        QCOMPARE(roles[TabWidgetsModel::TITLE],      QByteArray("title"));
        QCOMPARE(roles[TabWidgetsModel::TOPIC],      QByteArray("topic"));
        QCOMPARE(roles[TabWidgetsModel::TYPE],       QByteArray("type"));
        QCOMPARE(roles[TabWidgetsModel::COL],        QByteArray("column"));
        QCOMPARE(roles[TabWidgetsModel::ROW],        QByteArray("row"));
        QCOMPARE(roles[TabWidgetsModel::ROWSPAN],    QByteArray("rowSpan"));
        QCOMPARE(roles[TabWidgetsModel::COLSPAN],    QByteArray("colSpan"));
        QCOMPARE(roles[TabWidgetsModel::PROPERTIES], QByteArray("properties"));
        QCOMPARE(roles[TabWidgetsModel::IDX],        QByteArray("idx"));
    }
};

QTEST_GUILESS_MAIN(TestTabWidgetsModel)
#include "TestTabWidgetsModel.moc"
