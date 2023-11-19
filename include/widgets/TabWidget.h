#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QPoint>

class TabWidget : public QWidget
{
    Q_OBJECT
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
