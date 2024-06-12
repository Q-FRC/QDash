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

public:
    WidgetDialogGenerator(BaseWidget *widget, QWidget *parent = nullptr, bool isResize = false, WidgetData data = WidgetData{0, 0, 0, 0, 0});
    ~WidgetDialogGenerator();

    void bindMetaProperty(const QMetaProperty property, Getter getter);

    QVariantMap serializeTable(QTableWidget *widget);
    void serializeMap(QVariantMap map, QTableWidget *widget);

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
signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
};

bool operator<(const QMetaProperty a, const QMetaProperty b);

#endif // WIDGETDIALOGGENERATOR_H
