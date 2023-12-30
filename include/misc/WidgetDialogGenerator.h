#ifndef WIDGETDIALOGGENERATOR_H
#define WIDGETDIALOGGENERATOR_H

#include <QDialog>
#include <QTableWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QMetaProperty>

#include "Globals.h"

typedef std::function<QVariant()> Getter;
class BaseWidget;

class WidgetDialogGenerator : public QDialog
{
    Q_OBJECT
private:
    BaseWidget *m_widget;
    bool m_isResize;

    QFormLayout *m_layout;

    QLineEdit *m_nameInput;

    QSpinBox *m_rowInput;
    QSpinBox *m_columnInput;

    QSpinBox *m_rowSpanInput;
    QSpinBox *m_columnSpanInput;

    QDialogButtonBox *m_buttonBox;

    WidgetData m_data;

    QMultiMap<const QMetaProperty, Getter> m_propertyGetterMap;

    QMetaProperty m_currentProperty;
    BaseWidget *m_currentWidget;

    QPushButton *selectTopicButton();
    QPushButton *selectColorButton();

    std::tuple<QTableWidget *, QPushButton *, QPushButton *> setupTable(const QStringList &headers);

public:
    WidgetDialogGenerator(BaseWidget *widget, QWidget *parent = nullptr, bool isResize = false, WidgetData data = WidgetData{0, 0, 0, 1, 1});
    ~WidgetDialogGenerator();

    void bindMetaProperty(const QMetaProperty property, Getter getter);

    QVariantMap serializeTable(QTableWidget *widget);
    void serializeMap(QVariantMap map, QTableWidget *widget);
    void serializeMap(QHash<Globals::Topic, QColor> map, QTableWidget *widget);

    /** property functions **/
    QWidget *doubleProperty(QMetaProperty property);
    QWidget *intProperty(QMetaProperty property);

    QWidget *colorProperty(QMetaProperty property);

    QWidget *mapProperty(QMetaProperty property);

    QWidget *bitmapProperty(QMetaProperty property);
    QWidget *imageProperty(QMetaProperty property);

    QWidget *fileProperty(QMetaProperty property);

    QWidget *fontProperty(QMetaProperty property);

    QWidget *stringProperty(QMetaProperty property);

    QWidget *shapeProperty(QMetaProperty property);

    QWidget *topicListProperty(QMetaProperty property);

    QWidget *xAxisProperty(QMetaProperty property);

    QWidget *topicColorMapProperty(QMetaProperty property);
signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
    void cancelled(BaseWidget *widget);

    void topicSelected(const Globals::Topic &topic, QWidget *receiver);
    void colorSelected(const QColor &color);
};

bool operator<(const QMetaProperty a, const QMetaProperty b);

#endif // WIDGETDIALOGGENERATOR_H
