#include <QCoreApplication>
#include <QtTest>
#include "MainWindow.h"

// add necessary includes here

class SaveLoad : public QObject
{
    Q_OBJECT

public:
    SaveLoad();
    ~SaveLoad();

private:
    MainWindow *m_window;
    QJsonDocument m_doc;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSave();
};

SaveLoad::SaveLoad() {
    m_window = new MainWindow;
}

SaveLoad::~SaveLoad() {}

void SaveLoad::initTestCase() {
    QFile file("./test.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    m_doc = QJsonDocument::fromJson(file.readAll());
    m_window->loadObject(m_doc);
}

void SaveLoad::cleanupTestCase() {
    delete m_window;
}

void SaveLoad::testSave() {
    QJsonDocument doc = m_window->saveObject();
    QCOMPARE(doc, m_doc);
}

QTEST_MAIN(SaveLoad)

#include "tst_saveload.moc"
