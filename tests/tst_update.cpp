#include <QCoreApplication>
#include <QtTest>
#include "MainWindow.h"
#include "widgets/BaseWidget.h"
#include "networktables/NetworkTableInstance.h"

// Test to ensure deletion is safe & doesn't cause segfaults
class WidgetUpdate : public QObject
{
    Q_OBJECT

public:
    WidgetUpdate();
    ~WidgetUpdate();

private:
    MainWindow *m_window;
    nt::NetworkTableInstance m_inst;
    nt::NetworkTableEntry m_testEntry;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testUpdate();
};

WidgetUpdate::WidgetUpdate() {
    m_window = new MainWindow;
}

WidgetUpdate::~WidgetUpdate() {}

void WidgetUpdate::initTestCase() {
    QFile file("./test.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto doc = QJsonDocument::fromJson(file.readAll());
    m_window->loadObject(doc);

    m_inst = nt::NetworkTableInstance::GetDefault();
    m_inst.StartServer("", "0.0.0.0");

    m_testEntry = m_inst.GetEntry("/test");
    m_testEntry.SetBoolean(false);

    m_window->setNtSettings(ServerData{false, "0.0.0.0", NT_DEFAULT_PORT4});
    QTest::qWait(1000);
}

void WidgetUpdate::cleanupTestCase() {
    delete m_window;
    m_testEntry.Unpublish();
    m_inst.Disconnect();
}

void WidgetUpdate::testUpdate() {
    TabWidget *tab = m_window->currentTab();
    BaseWidget *widget = tab->widgets().at(0);
    m_testEntry.SetBoolean(true);

    QTest::qWait(1000);

    QCOMPARE(true, widget->property("value").toBool());
}

QTEST_MAIN(WidgetUpdate)

#include "tst_update.moc"
