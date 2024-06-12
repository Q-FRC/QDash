#pragma once

#include "Globals.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QLabel>
#include <QFormLayout>

class ResizeDialog : public QDialog
{
    Q_OBJECT
private:
    QFormLayout *m_layout;

    QLabel *m_rowLabel;
    QSpinBox *m_rowInput;

    QLabel *m_columnLabel;
    QSpinBox *m_columnInput;

    QLabel *m_rowSpanLabel;
    QSpinBox *m_rowSpanInput;

    QLabel *m_columnSpanLabel;
    QSpinBox *m_columnSpanInput;

    QDialogButtonBox *m_buttonBox;
public:
    ResizeDialog(WidgetData widgetData);
    virtual ~ResizeDialog();

signals:
    void resizeReady(WidgetData data);
};
