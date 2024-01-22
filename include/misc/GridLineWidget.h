#ifndef GRIDLINEWIDGET_H
#define GRIDLINEWIDGET_H

#include <QWidget>
#include <QPoint>

#include "Globals.h"

class GridLineWidget : public QWidget
{
    Q_OBJECT
private:
    QPoint m_size;

    WidgetData m_selection;

    bool m_hasSelection = false;
    bool m_isValidSelection = true;

public:
    explicit GridLineWidget(QWidget *parent = nullptr);

    void setSize(QPoint size);

    WidgetData selection();
    bool hasSelection();
    bool isValidSelection();

    void setSelection(const WidgetData &selectedIndex);
    void setHasSelection(const bool &hasSelection);
    void setValidSelection(const bool &isValidSelection);

    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // GRIDLINEWIDGET_H
