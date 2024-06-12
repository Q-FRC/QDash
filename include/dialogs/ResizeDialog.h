// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "widgets/TabWidget.h"
#include "widgets/BaseWidget.h"

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
    ResizeDialog(QList<int> widgetData);
    virtual ~ResizeDialog();

signals:
    void finished(QList<int> data);
};
