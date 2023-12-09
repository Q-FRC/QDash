#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QPoint>

#include "Globals.h"

class TabWidget : public QWidget
{
    Q_OBJECT
private:
    QGridLayout *m_layout;
    
    QPoint m_maxSize;
    WidgetData m_selectedIndex;

    bool m_hasSelection = false;
    bool m_isValidSelection = true;
public:
    TabWidget(const QPoint &maxSize, QWidget *parent = nullptr);
    virtual ~TabWidget();

    QGridLayout *layout();

    QPoint maxSize();
    void setMaxSize(const QPoint &maxSize);

    WidgetData selectedIndex();
    void setSelectedIndex(const WidgetData &selectedIndex);

    bool hasSelection();
    void setHasSelection(const bool &hasSelection);

    bool isValidSelection();
    void setValidSelection(const bool &isValidSelection);

    void paintEvent(QPaintEvent *event) override;
};
