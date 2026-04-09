// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtQuickTest>
#include <QObject>
#include <QJSValue>
#include <QVariant>
#include <QVariantMap>
#include <QPointF>
#include <QRectF>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlPropertyMap>

// -----------------------------------------------------------------------
// Mock context property objects
// All members are intentionally kept minimal – they only implement the
// surface that BaseWidget / PrimitiveWidget / SendableWidget call during
// normal instantiation and test-driven update() calls.
// -----------------------------------------------------------------------

/**
 * Stub for the `TopicStore` context property.
 *
 * subscribe/unsubscribe/forceUpdate/getValue/setValue are all no-ops so
 * that PrimitiveWidget and SendableWidget can call them safely without a
 * running NetworkTables instance.  The `connected(bool)` signal is present
 * so that the `Connections { target: TopicStore }` block in PrimitiveWidget
 * can bind without errors.
 */
class MockTopicStore : public QObject
{
    Q_OBJECT
    // Dummy property to keep moc from complaining about an otherwise
    // signal-only class.
    Q_PROPERTY(bool dummy READ dummy CONSTANT)
public:
    using QObject::QObject;

    Q_INVOKABLE void subscribe(const QString &, const QJSValue &) {}
    Q_INVOKABLE void unsubscribe(const QString &, const QJSValue &) {}
    Q_INVOKABLE void forceUpdate(const QString &) {}
    Q_INVOKABLE QVariant getValue(const QString &) { return {}; }
    Q_INVOKABLE void setValue(const QString &, const QVariant &) {}

    bool dummy() const { return false; }

signals:
    // PrimitiveWidget / SendableWidget bind to this signal.
    void connected(bool isConnected);
};

/**
 * Stub for the `QDashSettings` context property.
 *
 * Only `disableWidgets` is used by PrimitiveWidget / SendableWidget;
 * `scale` is included because BaseWidget references it indirectly via Clover.
 */
class MockQDashSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool disableWidgets MEMBER m_disableWidgets NOTIFY disableWidgetsChanged)
    Q_PROPERTY(double scale MEMBER m_scale NOTIFY scaleChanged)
public:
    using QObject::QObject;

    bool m_disableWidgets = false;
    double m_scale = 1.0;

signals:
    void disableWidgetsChanged();
    void scaleChanged();
};

/**
 * Stub for the `tab` context property.
 *
 * BaseWidget reads colWidth, rowHeight, rows, cols to compute its geometry.
 * It also writes to `latestWidget` in Component.onCompleted and listens to
 * `topicViewRectChanged`, `colWidthChanged`, `rowHeightChanged`.
 */
class MockTab : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double colWidth     READ colWidth     CONSTANT)
    Q_PROPERTY(double rowHeight    READ rowHeight    CONSTANT)
    Q_PROPERTY(int    rows         READ rows         CONSTANT)
    Q_PROPERTY(int    cols         READ cols         CONSTANT)
    Q_PROPERTY(QRectF topicViewRect READ topicViewRect CONSTANT)
    Q_PROPERTY(QObject *latestWidget
               READ  latestWidget
               WRITE setLatestWidget
               NOTIFY latestWidgetChanged)
public:
    using QObject::QObject;

    static constexpr double kColWidth  = 300.0;
    static constexpr double kRowHeight = 200.0;

    double colWidth()  const { return kColWidth;  }
    double rowHeight() const { return kRowHeight; }
    int    rows()      const { return 5; }
    int    cols()      const { return 5; }
    QRectF topicViewRect() const { return {}; }

    QObject *latestWidget() const { return m_latestWidget; }
    void setLatestWidget(QObject *w)
    {
        if (m_latestWidget != w) {
            m_latestWidget = w;
            emit latestWidgetChanged();
        }
    }

signals:
    void topicViewRectChanged();
    void colWidthChanged();
    void rowHeightChanged();
    void latestWidgetChanged();

private:
    QObject *m_latestWidget = nullptr;
};

/**
 * Stub for the `grid` context property.
 *
 * BaseWidget uses colWidth/rowHeight for geometry bindings and calls
 * validSpot/validResize/resetValid/getPoint during drag and resize events
 * (which don't occur in unit tests).
 */
class MockGrid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double colWidth  READ colWidth  CONSTANT)
    Q_PROPERTY(double rowHeight READ rowHeight CONSTANT)
public:
    using QObject::QObject;

    double colWidth()  const { return MockTab::kColWidth;  }
    double rowHeight() const { return MockTab::kRowHeight; }

    Q_INVOKABLE bool validSpot(double, double, int, int, int, int, bool) { return true; }
    Q_INVOKABLE bool validResize(double, double, double, double, int, int, int, int) { return true; }
    Q_INVOKABLE void resetValid() {}
    Q_INVOKABLE QPointF getPoint(double, double, bool) { return {0.0, 0.0}; }
    Q_INVOKABLE QRectF  getRect (double, double, double, double) { return {0, 0, 1, 1}; }
};

/**
 * Stub for the `twm` (TabWidgetsModel) context property.
 * BaseWidget's "Delete" menu item calls twm.remove(); it is never triggered
 * during tests so no-op is sufficient.
 */
class MockTwm : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    Q_INVOKABLE void remove(int, int) {}
    Q_INVOKABLE void remove(const QVariant &) {}
};

// -----------------------------------------------------------------------
// Test setup
// -----------------------------------------------------------------------

/**
 * Setup object passed to QUICK_TEST_MAIN_WITH_SETUP.
 *
 * qmlEngineAvailable() runs before any QML test file is loaded:
 *  1. Adds ${CMAKE_BINARY_DIR}/qml to the import path so all QDash.*
 *     module imports resolve.
 *  2. Sets mock context properties for every name that the widget base
 *     classes (BaseWidget, PrimitiveWidget, SendableWidget) reference:
 *     TopicStore, QDashSettings, tab, grid, twm, model.
 *  3. Also sets the bare Repeater role aliases (row, column, rowSpan,
 *     colSpan) that BaseWidget's drag/resize handlers use.
 */
class QDashWidgetTestSetup : public QObject
{
    Q_OBJECT

public:
    explicit QDashWidgetTestSetup() = default;

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        engine->addImportPath(QStringLiteral(QDASH_QML_PATH));

        auto *ctx = engine->rootContext();

        // Instantiate mock objects owned by the engine so they are
        // destroyed together with it.
        auto *mockStore    = new MockTopicStore(engine);
        auto *mockSettings = new MockQDashSettings(engine);
        auto *mockTab      = new MockTab(engine);
        auto *mockGrid     = new MockGrid(engine);
        auto *mockTwm      = new MockTwm(engine);

        // QQmlPropertyMap provides a QML-accessible object whose keys map
        // to the roles that BaseWidget / PrimitiveWidget read from `model`.
        // Key writes from the widget (e.g. `model.row = mrow`) are also
        // handled by QQmlPropertyMap via its property-value-changed path.
        auto *mockModel = new QQmlPropertyMap(engine);
        mockModel->insert(QStringLiteral("row"),        0);
        mockModel->insert(QStringLiteral("column"),     0);
        mockModel->insert(QStringLiteral("rowSpan"),    1);
        mockModel->insert(QStringLiteral("colSpan"),    1);
        mockModel->insert(QStringLiteral("title"),      QStringLiteral(""));
        mockModel->insert(QStringLiteral("topic"),      QStringLiteral(""));
        mockModel->insert(QStringLiteral("type"),       QStringLiteral(""));
        mockModel->insert(QStringLiteral("idx"),        0);
        mockModel->insert(QStringLiteral("properties"), QVariantMap{});

        // Primary context properties used by the widget base classes.
        ctx->setContextProperty(QStringLiteral("TopicStore"),    mockStore);
        ctx->setContextProperty(QStringLiteral("QDashSettings"), mockSettings);
        ctx->setContextProperty(QStringLiteral("tab"),           mockTab);
        ctx->setContextProperty(QStringLiteral("grid"),          mockGrid);
        ctx->setContextProperty(QStringLiteral("twm"),           mockTwm);
        ctx->setContextProperty(QStringLiteral("model"),         mockModel);

        // Bare Repeater role aliases used by BaseWidget's drag/resize
        // handlers (set in addition to the model map above).
        ctx->setContextProperty(QStringLiteral("row"),      0);
        ctx->setContextProperty(QStringLiteral("column"),   0);
        ctx->setContextProperty(QStringLiteral("rowSpan"),  1);
        ctx->setContextProperty(QStringLiteral("colSpan"),  1);
    }
};

QUICK_TEST_MAIN_WITH_SETUP(Widgets, QDashWidgetTestSetup)

#include "tst_Widgets.moc"
