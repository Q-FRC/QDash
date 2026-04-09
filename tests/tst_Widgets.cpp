// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtQuickTest/quicktest.h>
#include <QQmlContext>
#include <QQmlEngine>
#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QRectF>
#include <QPointF>

// QDash native headers
#include "Misc/Globals.h"
#include "NT/TopicStore.h"
#include "Logging/Logger.h"

// ntcore
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTableEntry.h>

/**
 * @brief TestNTHelper
 * Allows QML tests to publish values directly to the isolated NT instance
 * so that TopicStore listeners fire and widgets update.
 */
class TestNTHelper : public QObject {
    Q_OBJECT

public:
    explicit TestNTHelper(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void publishBool(const QString &topic, bool value) {
        Globals::inst.GetEntry(topic.toStdString())
            .SetValue(nt::Value::MakeBoolean(value));
    }

    Q_INVOKABLE void publishDouble(const QString &topic, double value) {
        Globals::inst.GetEntry(topic.toStdString())
            .SetValue(nt::Value::MakeDouble(value));
    }

    Q_INVOKABLE void publishString(const QString &topic, const QString &value) {
        Globals::inst.GetEntry(topic.toStdString())
            .SetValue(nt::Value::MakeString(value.toStdString()));
    }

    Q_INVOKABLE void publishInteger(const QString &topic, int value) {
        Globals::inst.GetEntry(topic.toStdString())
            .SetValue(nt::Value::MakeInteger(static_cast<int64_t>(value)));
    }
};

/**
 * @brief MockWidgetModel
 * Mocks the per-delegate model that BaseWidget/PrimitiveWidget read from.
 * BaseWidget accesses: row, column, rowSpan, colSpan, title, topic, type,
 * properties, idx.
 */
class MockWidgetModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(int row    READ row    WRITE setRow    NOTIFY rowChanged    FINAL)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged FINAL)
    Q_PROPERTY(int rowSpan READ rowSpan WRITE setRowSpan NOTIFY rowSpanChanged FINAL)
    Q_PROPERTY(int colSpan READ colSpan WRITE setColSpan NOTIFY colSpanChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString topic READ topic WRITE setTopic NOTIFY topicChanged FINAL)
    Q_PROPERTY(QString type  READ type  WRITE setType  NOTIFY typeChanged  FINAL)
    Q_PROPERTY(QVariantMap properties READ properties WRITE setProperties NOTIFY propertiesChanged FINAL)
    Q_PROPERTY(int idx READ idx WRITE setIdx NOTIFY idxChanged FINAL)

public:
    explicit MockWidgetModel(QObject *parent = nullptr) : QObject(parent) {}

    int row() const { return m_row; }
    void setRow(int v) { if (m_row != v) { m_row = v; emit rowChanged(); } }

    int column() const { return m_column; }
    void setColumn(int v) { if (m_column != v) { m_column = v; emit columnChanged(); } }

    int rowSpan() const { return m_rowSpan; }
    void setRowSpan(int v) { if (m_rowSpan != v) { m_rowSpan = v; emit rowSpanChanged(); } }

    int colSpan() const { return m_colSpan; }
    void setColSpan(int v) { if (m_colSpan != v) { m_colSpan = v; emit colSpanChanged(); } }

    QString title() const { return m_title; }
    void setTitle(const QString &v) { if (m_title != v) { m_title = v; emit titleChanged(); } }

    QString topic() const { return m_topic; }
    void setTopic(const QString &v) { if (m_topic != v) { m_topic = v; emit topicChanged(); } }

    QString type() const { return m_type; }
    void setType(const QString &v) { if (m_type != v) { m_type = v; emit typeChanged(); } }

    QVariantMap properties() const { return m_properties; }
    void setProperties(const QVariantMap &v) { m_properties = v; emit propertiesChanged(); }

    int idx() const { return m_idx; }
    void setIdx(int v) { if (m_idx != v) { m_idx = v; emit idxChanged(); } }

signals:
    void rowChanged();
    void columnChanged();
    void rowSpanChanged();
    void colSpanChanged();
    void titleChanged();
    void topicChanged();
    void typeChanged();
    void propertiesChanged();
    void idxChanged();

private:
    int m_row = 0;
    int m_column = 0;
    int m_rowSpan = 1;
    int m_colSpan = 1;
    QString m_title = "Test Widget";
    QString m_topic;
    QString m_type;
    QVariantMap m_properties;
    int m_idx = 0;
};

/**
 * @brief MockTab
 * Mocks the "tab" context property used by BaseWidget.
 */
class MockTab : public QObject {
    Q_OBJECT

    Q_PROPERTY(QRectF topicViewRect READ topicViewRect WRITE setTopicViewRect NOTIFY topicViewRectChanged FINAL)
    Q_PROPERTY(QObject *latestWidget READ latestWidget WRITE setLatestWidget NOTIFY latestWidgetChanged FINAL)
    Q_PROPERTY(double colWidth READ colWidth NOTIFY colWidthChanged FINAL)
    Q_PROPERTY(double rowHeight READ rowHeight NOTIFY rowHeightChanged FINAL)

public:
    explicit MockTab(QObject *parent = nullptr) : QObject(parent) {}

    QRectF topicViewRect() const { return m_topicViewRect; }
    void setTopicViewRect(const QRectF &r) { m_topicViewRect = r; emit topicViewRectChanged(); }

    QObject *latestWidget() const { return m_latestWidget; }
    void setLatestWidget(QObject *w) { m_latestWidget = w; emit latestWidgetChanged(); }

    double colWidth() const { return 200.0; }
    double rowHeight() const { return 150.0; }

signals:
    void topicViewRectChanged();
    void latestWidgetChanged();
    void colWidthChanged();
    void rowHeightChanged();

private:
    QRectF m_topicViewRect;
    QObject *m_latestWidget = nullptr;
};

/**
 * @brief MockGrid
 * Mocks the "grid" Repeater context property used by BaseWidget.
 */
class MockGrid : public QObject {
    Q_OBJECT

    Q_PROPERTY(double colWidth  READ colWidth  CONSTANT FINAL)
    Q_PROPERTY(double rowHeight READ rowHeight CONSTANT FINAL)

public:
    explicit MockGrid(QObject *parent = nullptr) : QObject(parent) {}

    double colWidth() const { return 200.0; }
    double rowHeight() const { return 150.0; }

    Q_INVOKABLE bool validSpot(double, double, int, int, int, int, bool) { return true; }
    Q_INVOKABLE void resetValid() {}
    Q_INVOKABLE void validResize(double, double, double, double, int, int, int, int) {}
    Q_INVOKABLE QPointF getPoint(double, double, bool) { return QPointF(0, 0); }
};

/**
 * @brief MockTwm
 * Mocks the "twm" context property. BaseWidget calls twm.remove() with either
 * one argument (idx) or two (row, column) depending on the call site.
 */
class MockTwm : public QObject {
    Q_OBJECT

public:
    explicit MockTwm(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void remove(int, int = -1) {}
    Q_INVOKABLE bool cellOccupied(int, int, int, int, QRectF) { return false; }
};

/**
 * @brief MockSettings
 * Mocks the "QDashSettings" context property used by PrimitiveWidget.
 */
class MockSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool disableWidgets READ disableWidgets NOTIFY disableWidgetsChanged FINAL)

public:
    explicit MockSettings(QObject *parent = nullptr) : QObject(parent) {}

    bool disableWidgets() const { return false; }

signals:
    void disableWidgetsChanged();
};

/**
 * @brief QDashWidgetTestSetup
 * QUICK_TEST_MAIN_WITH_SETUP setup class.
 *
 * - applicationAvailable(): replaces Globals::inst with a fresh isolated NT
 *   instance so tests do not connect to a real robot or DS.
 * - qmlEngineAvailable(): injects a real TopicStore (backed by the test NT
 *   instance) plus a TestNTHelper and the remaining mock context properties.
 */
class QDashWidgetTestSetup : public QObject {
    Q_OBJECT

    Logger         *m_logger     = nullptr;
    TopicStore     *m_topicStore = nullptr;
    TestNTHelper   *m_ntHelper   = nullptr;
    MockWidgetModel *m_model     = nullptr;
    MockTab        *m_tab        = nullptr;
    MockGrid       *m_grid       = nullptr;
    MockTwm        *m_twm        = nullptr;
    MockSettings   *m_settings   = nullptr;

public:
    explicit QDashWidgetTestSetup(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void applicationAvailable() {
        // Replace the global NT instance with a fresh isolated one so that no
        // network connections are made and published values are strictly local.
        Globals::inst = nt::NetworkTableInstance::Create();
    }

    void qmlEngineAvailable(QQmlEngine *engine) {
        m_logger     = new Logger(this);
        m_topicStore = new TopicStore(engine, m_logger, this);
        m_ntHelper   = new TestNTHelper(this);
        m_model      = new MockWidgetModel(this);
        m_tab        = new MockTab(this);
        m_grid       = new MockGrid(this);
        m_twm        = new MockTwm(this);
        m_settings   = new MockSettings(this);

        auto *ctx = engine->rootContext();
        ctx->setContextProperty("TopicStore",    m_topicStore);
        ctx->setContextProperty("ntHelper",      m_ntHelper);
        ctx->setContextProperty("model",         m_model);
        ctx->setContextProperty("tab",           m_tab);
        ctx->setContextProperty("grid",          m_grid);
        ctx->setContextProperty("twm",           m_twm);
        ctx->setContextProperty("QDashSettings", m_settings);
    }

    void cleanupTestCase() {
        // Destroy the isolated NT instance after all tests finish.
        Globals::inst.Destroy();
    }
};

QUICK_TEST_MAIN_WITH_SETUP(tst_Widgets, QDashWidgetTestSetup)

#include "tst_Widgets.moc"

