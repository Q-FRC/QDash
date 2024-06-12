#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QRect>

#include "Globals.h"
#include "widgets/BaseWidget.h"

class TabWidget : public QWidget
{
    Q_OBJECT
private:
    QGridLayout *m_layout;
    
    QPoint m_maxSize;
    WidgetData m_selectedIndex;

    bool m_hasSelection = false;
    bool m_isValidSelection = true;

    QString m_name;

    QList<BaseWidget *> m_widgets;

    // drag and drop

    // Dragging
    QPoint m_dragStart;
    QPoint m_dragOffset;
    BaseWidget *m_draggedWidget;
    WidgetData m_draggedWidgetData;
    bool m_dragging = false;

    void dragMove(QPoint point);
    void dragRelease(QPoint point);

    // Resizing
    ResizeDirection m_currentResize;
    QRect m_initialSize;
    bool m_resizing = false;

    void resizeStart(QPoint point);
    void resizeMove(QPoint point);
    void resizeRelease(QPoint point);

    // Events
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    TabWidget(const QPoint &maxSize, QWidget *parent = nullptr);
    virtual ~TabWidget();

    QList<BaseWidget *> widgets();

    bool widgetAtPoint(WidgetData data);

    WidgetData widgetData(BaseWidget *widget);

    QGridLayout *layout();

    QString name() const;
    void setName(const QString &newName);

    QPoint maxSize();
    void setMaxSize(const QPoint &maxSize);

    WidgetData selectedIndex();
    void setSelectedIndex(const WidgetData &selectedIndex);

    bool hasSelection();
    void setHasSelection(const bool &hasSelection);

    bool isValidSelection();
    void setValidSelection(const bool &isValidSelection);

    QJsonObject saveObject();
    void loadObject(const QJsonObject &object);

    // drag and drop epic edition
    void setDragData(BaseWidget *widget, WidgetData data);
    void dragStart(QPoint point, QPoint offset);

public slots:
    void addWidget(BaseWidget *widget, WidgetData data);
    void deleteWidget(BaseWidget *widget);

signals:
    void dragDone(BaseWidget *widget, WidgetData data);

};
