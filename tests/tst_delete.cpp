#include <QCoreApplication>
#include <QtTest>
#include "MainWindow.h"
#include "widgets/BaseWidget.h"
#include "networktables/NetworkTableInstance.h"

// Test to ensure deletion is safe & doesn't cause segfaults
class WidgetDeletion : public QObject
{
    Q_OBJECT

public:
    WidgetDeletion();
    ~WidgetDeletion();

private:
    MainWindow *m_window;
    nt::NetworkTableInstance m_inst;
    nt::NetworkTableEntry m_testEntry;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDelete();
};

WidgetDeletion::WidgetDeletion() {
    m_window = new MainWindow;
}

WidgetDeletion::~WidgetDeletion() {}

void WidgetDeletion::initTestCase() {
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

void WidgetDeletion::cleanupTestCase() {
    delete m_window;
    m_testEntry.Unpublish();
    m_inst.Disconnect();
}

void WidgetDeletion::testDelete() {
    TabWidget *tab = m_window->currentTab();
    BaseWidget *widget = tab->widgets().at(0);
    tab->deleteWidget(widget);
    m_testEntry.SetBoolean(true);

    QTest::qWait(1000);
    Q_ASSERT(true);
}

QTEST_MAIN(WidgetDeletion)

#include "tst_delete.moc"
