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
public:
    WidgetDialogGenerator(BaseWidget *widget, bool isResize = false, WidgetData data = WidgetData{0, 0, 0, 0, 0});
    ~WidgetDialogGenerator();

    void bindMetaProperty(const QMetaProperty property, Getter getter);

    QVariantMap serializeTable(QTableWidget *widget);
    void serializeMap(QVariantMap map, QTableWidget *widget);
signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
};

bool operator<(const QMetaProperty a, const QMetaProperty b);

#endif // WIDGETDIALOGGENERATOR_H
