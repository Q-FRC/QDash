// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
#include "Models/AccentsListModel.h"

class TestAccentsListModel : public QObject {
    Q_OBJECT

private:
    // Build a QJsonDocument with the given accent entries.
    static QJsonDocument makeDoc(
        const QList<std::tuple<QString, QString, QString, QString>> &entries)
    {
        QJsonArray arr;
        for (const auto &[name, accent, light, qml] : entries) {
            QJsonObject obj;
            obj["name"]   = name;
            obj["accent"] = accent;
            obj["light"]  = light;
            obj["qml"]    = qml;
            arr.append(obj);
        }
        return QJsonDocument(arr);
    }

private slots:
    // ── initial state ──────────────────────────────────────────────────────
    void testInitialRowCount()
    {
        AccentsListModel model;
        QCOMPARE(model.rowCount(), 0);
    }

    // ── loadObject ─────────────────────────────────────────────────────────
    void testLoadObjectIncreasesRowCount()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QCOMPARE(model.rowCount(), 1);
    }

    void testLoadObjectMultipleEntries()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({
            {"red",  "#f00", "#ff9999", "Red.qml"},
            {"blue", "#00f", "#9999ff", "Blue.qml"},
        }));
        QCOMPARE(model.rowCount(), 2);
    }

    void testLoadObjectEmptyDocumentNoOp()
    {
        AccentsListModel model;
        model.loadObject(QJsonDocument(QJsonArray{}));
        QCOMPARE(model.rowCount(), 0);
    }

    void testLoadObjectAppendsOnSecondCall()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        model.loadObject(makeDoc({{"blue", "#00f", "#9999ff", "Blue.qml"}}));
        QCOMPARE(model.rowCount(), 2);
    }

    // ── data roles ─────────────────────────────────────────────────────────
    void testDataName()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"green", "#0f0", "#99ff99", "Green.qml"}}));
        QCOMPARE(model.data(model.index(0), AccentsListModel::NAME).toString(), "green");
    }

    void testDataAccent()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"green", "#0f0", "#99ff99", "Green.qml"}}));
        QCOMPARE(model.data(model.index(0), AccentsListModel::ACCENT).toString(), "#0f0");
    }

    void testDataLight()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"green", "#0f0", "#99ff99", "Green.qml"}}));
        QCOMPARE(model.data(model.index(0), AccentsListModel::LIGHT).toString(), "#99ff99");
    }

    void testDataQml()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"green", "#0f0", "#99ff99", "Green.qml"}}));
        QCOMPARE(model.data(model.index(0), AccentsListModel::QML).toString(), "Green.qml");
    }

    void testDataIdx()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({
            {"red",   "#f00", "#ff9999", "Red.qml"},
            {"blue",  "#00f", "#9999ff", "Blue.qml"},
            {"green", "#0f0", "#99ff99", "Green.qml"},
        }));
        QCOMPARE(model.data(model.index(0), AccentsListModel::IDX).toInt(), 0);
        QCOMPARE(model.data(model.index(1), AccentsListModel::IDX).toInt(), 1);
        QCOMPARE(model.data(model.index(2), AccentsListModel::IDX).toInt(), 2);
    }

    void testDataInvalidIndexReturnsNull()
    {
        AccentsListModel model;
        QCOMPARE(model.data(QModelIndex(), AccentsListModel::NAME), QVariant());
    }

    void testDataUnknownRoleReturnsNull()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QCOMPARE(model.data(model.index(0), Qt::DecorationRole), QVariant());
    }

    // ── accent() / light() / qml() lookups ────────────────────────────────
    void testAccentLookupFound()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QCOMPARE(model.accent("red"), "#f00");
    }

    void testAccentLookupNotFound()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QCOMPARE(model.accent("nonexistent"), "");
    }

    void testLightLookupFound()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QCOMPARE(model.light("red"), "#ff9999");
    }

    void testLightLookupNotFound()
    {
        AccentsListModel model;
        QCOMPARE(model.light("nonexistent"), "");
    }

    void testQmlLookupFound()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QCOMPARE(model.qml("red"), "Red.qml");
    }

    void testQmlLookupNotFound()
    {
        AccentsListModel model;
        QCOMPARE(model.qml("nonexistent"), "");
    }

    void testLookupFindsCorrectEntry()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({
            {"red",  "#f00", "#ff9999", "Red.qml"},
            {"blue", "#00f", "#9999ff", "Blue.qml"},
        }));
        QCOMPARE(model.accent("blue"),  "#00f");
        QCOMPARE(model.accent("red"),   "#f00");
    }

    // ── names() ────────────────────────────────────────────────────────────
    void testNamesReturnsAllNames()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({
            {"red",  "#f00", "#ff9999", "Red.qml"},
            {"blue", "#00f", "#9999ff", "Blue.qml"},
        }));
        QStringList names = model.names();
        QCOMPARE(names.size(), 2);
    }

    void testNamesAreTitleCase()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"red", "#f00", "#ff9999", "Red.qml"}}));
        QStringList names = model.names();
        // First character should be uppercased
        QCOMPARE(names[0][0], QChar('R'));
        QCOMPARE(names[0].mid(1), QString("ed"));
    }

    void testNamesEmptyModelReturnsEmptyList()
    {
        AccentsListModel model;
        QCOMPARE(model.names().size(), 0);
    }

    void testNamesPreservesRemainingCharacters()
    {
        AccentsListModel model;
        model.loadObject(makeDoc({{"purple", "#800080", "#cc99cc", "Purple.qml"}}));
        QCOMPARE(model.names()[0], QString("Purple"));
    }

    // ── roleNames ──────────────────────────────────────────────────────────
    void testRoleNames()
    {
        AccentsListModel model;
        QHash<int, QByteArray> roles = model.roleNames();
        QCOMPARE(roles[AccentsListModel::NAME],   QByteArray("name"));
        QCOMPARE(roles[AccentsListModel::ACCENT], QByteArray("accent"));
        QCOMPARE(roles[AccentsListModel::LIGHT],  QByteArray("light"));
        QCOMPARE(roles[AccentsListModel::QML],    QByteArray("qml"));
        QCOMPARE(roles[AccentsListModel::IDX],    QByteArray("idx"));
    }
};

QTEST_GUILESS_MAIN(TestAccentsListModel)
#include "TestAccentsListModel.moc"
