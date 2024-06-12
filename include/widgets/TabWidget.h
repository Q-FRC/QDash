// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QPoint>

class TabWidget : public QWidget
{
private:
    QGridLayout *m_layout;
    
    QPoint m_maxSize;

    void updateSpacers();
public:
    TabWidget(const QPoint &maxSize);
    virtual ~TabWidget();

    QGridLayout *layout();

    QPoint maxSize();
    void setMaxSize(const QPoint &maxSize);
};
