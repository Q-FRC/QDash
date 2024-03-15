#include "widgets/TabWidget.h"

#include <QJsonArray>
#include <QApplication>
#include <QMenu>
#include <QShortcut>

TabWidget::TabWidget(const QPoint &maxSize, QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::ClickFocus);

    m_layout = new QGridLayout(this);
    m_gridLine = new GridLineWidget(this);
    m_gridLine->lower();

    m_layout->addWidget(m_gridLine, 0, 0, -1, -1);

    // shortcuts
    {
        new QShortcut(QKeySequence(Qt::Key_Escape), this, [this] {
            cancelDrags();
            m_gridLine->setHasSelection(false);
            m_gridLine->setValidSelection(false);
            m_gridLine->update();

            emit dragCancelled(m_draggedWidget);
        });
    }

    setMaxSize(maxSize);
}

TabWidget::~TabWidget() {}

QList<BaseWidget *> TabWidget::widgets() {
    return m_widgets;
}

void TabWidget::addWidget(BaseWidget *widget, WidgetData data) {
    if (m_widgets.contains(widget)) {
        m_layout->removeWidget(widget);
    } else {
        m_widgets.append(widget);
        widget->setParent(this);
    }
    m_layout->addWidget(widget, data.row, data.col, data.rowSpan, data.colSpan);
    m_gridLine->lower();
}

void TabWidget::deleteWidget(BaseWidget *widget) {
    if (m_widgets.contains(widget)) {
        m_widgets.removeAll(widget);
        m_layout->removeWidget(widget);
        widget->deleteLater();
    }
}

void TabWidget::forceUpdateWidgets() {
    for (BaseWidget *widget : m_widgets) {
        widget->forceUpdate();
    }
}

WidgetData TabWidget::widgetData(BaseWidget *widget) {
    int idx = m_layout->indexOf(widget);
    if (idx == -1) {
        return WidgetData{0, 0, 0, 0};
    }

    WidgetData data;

    m_layout->getItemPosition(idx,
                              &data.row,
                              &data.col,
                              &data.rowSpan,
                              &data.colSpan);

    return data;
}

bool TabWidget::widgetAtPoint(WidgetData data) {
    // this is mild cancer but idrc

    for (int i = 0; i < m_layout->count(); ++i) {
        QLayoutItem *item = m_layout->itemAt(i);
        if (item->widget() == m_gridLine) continue;

        int row, col, rowSpan, colSpan;
        m_layout->getItemPosition(i, &row, &col, &rowSpan, &colSpan);

        QRect dataRect = QRect(data.row, data.col, data.rowSpan, data.colSpan);
        QRect itemRect = QRect(row, col, rowSpan, colSpan);

        if (dataRect.intersects(itemRect)) {
            return true;
        }
    }

    return false;
}

QGridLayout *TabWidget::layout() {
    return m_layout;
}

QString TabWidget::name() const
{
    return m_name;
}

void TabWidget::setName(const QString &newName)
{
    m_name = newName;
}

QPoint TabWidget::maxSize() {
    return m_maxSize;
}

void TabWidget::setMaxSize(const QPoint &maxSize) {
    QPoint lastMax = m_maxSize;
    m_maxSize = maxSize;

    m_gridLine->setSize(maxSize);

    if (lastMax.x() > maxSize.x()) {
        for (int x = maxSize.x(); x < lastMax.x(); ++x) {
            m_layout->setColumnStretch(x, 0);
        }
    } else {
        for (int x = 0; x < m_maxSize.x(); ++x) {
            m_layout->setColumnStretch(x, 1);
        }
    }

    if (lastMax.y() > maxSize.y()) {
        for (int y = maxSize.y(); y < lastMax.y(); ++y) {
            m_layout->setRowStretch(y, 0);
        }
    } else {
        for (int y = 0; y < m_maxSize.y(); ++y) {
            m_layout->setRowStretch(y, 1);
        }
    }
}

QJsonObject TabWidget::saveObject() {
    QJsonObject object{};
    object.insert("tabName", m_name);

    QPoint maxSize = m_maxSize;
    object.insert("maxSize", QJsonArray{ maxSize.x(), maxSize.y() });

    QJsonArray widgets{};

    for (BaseWidget *widget : m_widgets) {
        QJsonObject widgetObj = widget->saveObject();
        WidgetData data = widgetData(widget);

        widgetObj.insert("geometry", QJsonArray({data.row, data.col, data.rowSpan, data.colSpan}));

        widgets.append(widgetObj);
    }

    object.insert("widgets", widgets);
    return object;
}

void TabWidget::loadObject(const QJsonObject &object) {
    QString name = object.value("tabName").toString();
    m_name = name;

    QJsonArray maxSize = object.value("maxSize").toArray();
    setMaxSize(QPoint(maxSize.at(0).toInt(3), maxSize.at(1).toInt(3)));

    QJsonArray widgets = object.value("widgets").toArray();

    for (QJsonValueRef wref : widgets) {
        QJsonObject widgetObject = wref.toObject();

        QString topic = widgetObject.value("Topic").toString("");

        WidgetTypes type = (WidgetTypes) widgetObject.value("widgetType").toInt();
        BaseWidget *widget = BaseWidget::defaultWidgetFromTopic(topic, type);

        WidgetData data = widget->fromJson(widgetObject);

        addWidget(widget, data);
    } // widgets
}

/* DRAG AND DROP */

void TabWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    BaseWidget *widgetPressed = nullptr;

    for (BaseWidget *widget : widgets())
    {
        // map to tab widget as the base widget's geometry is relative to the tab widget
        if (widget->geometry().contains(mapFromGlobal(event->globalPosition()).toPoint())) {
            widgetPressed = widget;
            break;
        }
    }

    if (!widgetPressed) {

        return;
    }
    if (event->button() == Qt::RightButton) {
        QMenu *menu = widgetPressed->constructContextMenu(widgetData(widgetPressed));

        connect(widgetPressed, &BaseWidget::reconfigRequested, this, [this](BaseWidget *widget, WidgetData data) {
            addWidget(widget, data);
        });

        connect(widgetPressed, &BaseWidget::deleteRequested, this, [this, widgetPressed] {
            deleteWidget(widgetPressed);
        });

        menu->popup(event->globalPosition().toPoint());
        return;
    }

    if (event->button() != Qt::LeftButton) {
        return;
    }

    setDragData(widgetPressed, widgetData(widgetPressed));

    m_dragStart = event->pos();
    widgetPressed->raise();

    if (widgetPressed->resizing() == NONE) {
        dragStart(event->pos(), event->pos());
    } else {
        resizeStart(event->pos());
    }
}

void TabWidget::mouseMoveEvent(QMouseEvent *event) {
    if ((event->pos() - m_dragStart).manhattanLength()
        < QApplication::startDragDistance())
        return;

    QPoint point = mapFromGlobal(event->globalPosition()).toPoint();

    if (m_dragging) {
        dragMove(point);
    } else if (m_resizing) {
        resizeMove(point);
    }
}

void TabWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (!m_gridLine->hasSelection()) goto end;

    if (m_gridLine->isValidSelection()) {
        m_draggedWidgetData = m_gridLine->selection();
    }

    if (m_dragging) {
        emit dragDone(m_draggedWidget, m_draggedWidgetData);
    } else if (!m_resizing) {
        goto end;
    }

    addWidget(m_draggedWidget,
              m_draggedWidgetData);

end:
    cancelDrags();
}

void TabWidget::setDragData(BaseWidget *widget, WidgetData data) {
    cancelDrags();
    emit dragCancelled(m_draggedWidget);

    m_draggedWidget = widget;
    m_draggedWidgetData = data;
}

void TabWidget::cancelDrags() {
    // bunch of ways to ensure the widget is actually there
    if (
        (m_dragging || m_resizing) &&
        (hasWidget(m_draggedWidget) || m_draggedWidget->isVisible())) {
        addWidget(m_draggedWidget,
                  m_draggedWidgetData);
    }

    m_dragging = false;
    m_resizing = false;

    m_draggedWidget = nullptr;

    m_gridLine->setHasSelection(false);
}

void TabWidget::dragStart(QPoint point, QPoint offset) {
    if (offset == point) {
        m_dragOffset = offset - m_draggedWidget->pos();
    } else {
        m_dragOffset = offset;
    }
    m_dragging = true;
}

void TabWidget::dragMove(QPoint point) {
    if (m_draggedWidget && m_draggedWidget->isVisible() && m_draggedWidget->isEnabled()) {
        QPoint offset = point - m_dragOffset;
        m_draggedWidget->move(offset);

        if (!m_gridLine->hasSelection()) {
            m_layout->removeWidget(m_draggedWidget);
            m_widgets.removeAll(m_draggedWidget);
        }
    }

    int col = std::floor(point.x() / (width() / maxSize().x()));
    int row = std::floor(point.y() / (height() / maxSize().y()));

    int colSpan = m_draggedWidgetData.colSpan;
    int rowSpan = m_draggedWidgetData.rowSpan;

    WidgetData data{row, col, rowSpan, colSpan};

    m_gridLine->setValidSelection(!widgetAtPoint(data) &&
                                  (row + rowSpan <= m_maxSize.y()) &&
                                  (row >= 0) &&
                                  (col + colSpan <= m_maxSize.x()) &&
                                  (col >= 0));

    m_gridLine->setSelection(data);
}

void TabWidget::resizeStart(QPoint point) {
    m_currentResize = m_draggedWidget->resizing();
    m_initialSize = m_draggedWidget->geometry();

    setCursor(m_draggedWidget->cursor());
    m_resizing = true;
}

void TabWidget::resizeMove(QPoint point) {
    if (m_initialSize == QRect())
        return;

    if (!m_gridLine->hasSelection()) {
        layout()->removeWidget(m_draggedWidget);
        m_widgets.removeAll(m_draggedWidget);
    }

    QPoint offset = point - m_dragStart;
    ResizeDirection dir = m_currentResize;

    int dx = 0;
    int dy = 0;
    int dh = 0;
    int dw = 0;

    if (dir & LEFT) {
        dx = offset.x();
        dw = -offset.x();
    }
    if (dir & RIGHT) {
        dw = offset.x();
    }
    if (dir & TOP) {
        dy = offset.y();
        dh = -offset.y();
    }
    if (dir & BOTTOM) {
        dh = offset.y();
    }

    int colF = std::floor(point.x() / (width() / m_maxSize.x()));
    int rowF = std::floor(point.y() / (height() / m_maxSize.y()));

    int colSpanI = m_draggedWidgetData.colSpan;
    int rowSpanI = m_draggedWidgetData.rowSpan;

    int colI = m_draggedWidgetData.col;
    int rowI = m_draggedWidgetData.row;

    if (dir & LEFT) colI += colSpanI - 1;
    if (dir & TOP) rowI += rowSpanI - 1;

    int col = dir & LEFT ? std::min(colF, colI) : colI;
    int row = dir & TOP ? std::min(rowF, rowI) : rowI;

    int colSpan = (dir & LEFT || dir & RIGHT) ? (dir & RIGHT ? std::abs(colF - colI) + 1 : std::abs(colI - colF) + 1) : colSpanI;
    int rowSpan = (dir & TOP || dir & BOTTOM) ? (dir & BOTTOM ? std::abs(rowF - rowI) + 1 : std::abs(rowI - rowF) + 1) : rowSpanI;

    if (colSpan < 1) colSpan = 1;
    if (rowSpan < 1) rowSpan = 1;

    if (dw <= -m_initialSize.width()) {
        dw = 0;
        colSpan = 1;

    }
    if (dh <= -m_initialSize.height()) {
        dh = 0;
        rowSpan = 1;
    }

    m_draggedWidget->setGeometry(
        m_initialSize.x() + dx,
        m_initialSize.y() + dy,
        m_initialSize.width() + dw,
        m_initialSize.height() + dh);

    WidgetData data{row, col, rowSpan, colSpan};

    m_gridLine->setSelection(data);

    m_gridLine->setValidSelection(!widgetAtPoint(data) &&
                                  (row + rowSpan <= m_maxSize.y()) &&
                                  (row >= 0) &&
                                  (col + colSpan <= m_maxSize.x()) &&
                                  (col >= 0));
}

bool TabWidget::hasWidget(BaseWidget *widget) {
    return (widgetData(widget) != WidgetData{0, 0, 0, 0});
}
