#include "widgets/GraphWidget.h"
#include "stores/TopicStore.h"
#include "stores/FilterStore.h"
#include "misc/ChartView.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

GraphWidget::GraphWidget(const QString &topic, const QString &title)
    : BaseWidget(WidgetType, title, topic, true) {

    QChart *chart = new QChart();
    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_chart = new ChartView(chart, this);
    m_elapsed.start();

    connect(m_timer, &QTimer::timeout, this, &GraphWidget::updateGraph);

    m_xAxis = new QValueAxis(this);
    m_xAxis->setTitleText("Time");

    m_yAxis = new QValueAxis(this);
    m_yAxis->setTitleText("Value");

    m_chart->chart()->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->chart()->addAxis(m_yAxis, Qt::AlignLeft);

    setMaxTimeScale(30.);

    setMaxYValue(10.0);
    setMinYValue(-10.0);

    m_layout->addWidget(m_chart, 1, 0, 3, 1);
}

QHash<Globals::NumberTopic, QColor> GraphWidget::topics() {
    QHash<Globals::NumberTopic, QColor> newHash{};

    QHashIterator iter(m_topics);
    while(iter.hasNext()) {
        iter.next();
        Globals::NumberTopic t;
        t = iter.key();
        newHash.insert(t, iter.value());
    }
    return newHash;
}

void GraphWidget::setTopics(QHash<Globals::NumberTopic, QColor> topics) {
    QHash<Globals::Topic, QColor> newHash{};

    QHashIterator t_iter(topics);
    while(t_iter.hasNext()) {
        t_iter.next();
        newHash.insert(t_iter.key(), t_iter.value());
    }

    QHashIterator iter(m_topics);

    while (iter.hasNext()) {
        iter.next();
        Globals::Topic topic = iter.key();

        if (!newHash.contains(topic)) {
            TopicStore::unsubscribe(topic.Name.toStdString(), this);
            m_entryMap.remove(topic);

            QLineSeries *series = m_seriesMap.take(topic);
            m_chart->chart()->removeSeries(series);
            delete series;
        }
    }

    iter = QHashIterator(newHash);

    while (iter.hasNext()) {
        iter.next();
        Globals::Topic topic = iter.key();

        if (!m_topics.contains(topic)) {
            nt::NetworkTableEntry *entry = TopicStore::subscribe(topic.Name.toStdString(), this, NT_UNASSIGNED, topic.Name, true);

            m_entryMap.insert(topic, entry);

            QLineSeries *series = new QLineSeries;
            series->setName(topic.Name);
            series->append(m_elapsed.elapsed() / 1000., TopicStore::getDoubleFromEntry(entry));
            series->setColor(iter.value());

            m_chart->chart()->addSeries(series);
            series->attachAxis(m_xAxis);
            series->attachAxis(m_yAxis);
            series->setMarkerSize(100);

            m_seriesMap.insert(topic, series);
        } else {
            QLineSeries *series = m_seriesMap.value(topic);
            series->setColor(iter.value());
        }
    }

    m_topics = newHash;
}


double GraphWidget::maxTimeScale() {
    return m_maxTimeScale;
}

void GraphWidget::setMaxTimeScale(double maxTimeScale) {
    m_maxTimeScale = maxTimeScale;
}

double GraphWidget::updateFrequency() {
    return m_updateFrequency;
}

void GraphWidget::setUpdateFrequency(double frequency) {
    m_updateFrequency = frequency;

    m_timer->stop();
    m_timer->start(frequency * 1000.);
}


double GraphWidget::maxYValue() {
    return m_maxYValue;
}

void GraphWidget::setMaxYValue(double max) {
    m_maxYValue = max;
    m_yAxis->setMax(max);
}

double GraphWidget::minYValue() {
    return m_minYValue;
}

void GraphWidget::setMinYValue(double min) {
    m_minYValue = min;
    m_yAxis->setMin(min);
}

Globals::GraphXAxis GraphWidget::xAxisData() {
    return m_xAxisData;
}

void GraphWidget::setXAxisData(const Globals::GraphXAxis &axis) {
    if (m_xAxisData != axis) {
        m_xAxis->setTitleText(axis.useTime ? "Time" : axis.topic);

        if (axis.useTime) {
            m_elapsed.restart();
        }

        if (m_xAxisEntry != nullptr) TopicStore::unsubscribe(m_xAxisData.topic.toStdString(), this);
        m_xAxisEntry = TopicStore::subscribe(axis.topic.toStdString(), this, NT_UNASSIGNED, axis.topic, true);

        for (QLineSeries *series : m_seriesMap.values()) series->clear();

        // reset values to NaN
        m_minXValue = std::nan("0");
        m_maxXValue = std::nan("1");

        m_xAxisData = axis;
    }
}

double GraphWidget::getCurrentXAxis() {
    if (m_xAxisData.useTime) {
        return m_elapsed.elapsed() / 1000.;
    }
    return TopicStore::getDoubleFromEntry(m_xAxisEntry);
}

void GraphWidget::updateGraph() {
    if (!m_ready) return;

    double xValue = getCurrentXAxis(), lowerBound;
    if (m_xAxisData.useTime) {
        lowerBound = xValue - m_maxTimeScale;
        if (lowerBound < 0) lowerBound = 0;

        m_minXValue = lowerBound;
        m_maxXValue = xValue;
    }
    else {
        if (std::isnan(m_maxXValue) || xValue >= m_maxXValue) m_maxXValue = xValue + 0.1;
        if (std::isnan(m_minXValue) || xValue <= m_minXValue) m_minXValue = xValue - 0.1;
    }

    m_xAxis->setRange(m_minXValue, m_maxXValue);

    QHashIterator iter(m_topics);

    while (iter.hasNext()) {
        iter.next();
        Globals::Topic topic = iter.key();

        nt::NetworkTableEntry *entry = m_entryMap.value(topic);
        QLineSeries *series = m_seriesMap.value(topic);

        double value = TopicStore::getDoubleFromEntry(entry);

        series->append(xValue, value);
    }

    update();
}

QMenu *GraphWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = BaseWidget::constructContextMenu(data);

    QAction *csvAction = new QAction("Export CSV...", menu);
    menu->addAction(csvAction);

    connect(csvAction, &QAction::triggered, this, &GraphWidget::exportCSVPopup);

    return menu;
}


void GraphWidget::mouseReleaseEvent(QMouseEvent *event) {
    return event->ignore();
}

void GraphWidget::exportCSVPopup() {
    QString fileString = QFileDialog::getSaveFileName(this, "Export CSV To...", QDir::homePath(), "CSV Files (*.csv);;All Files (*)");

    exportCSV(fileString);
}

void GraphWidget::exportCSV(const QString &fileName) {
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        QString xLabel = m_xAxisData.useTime ? "Time" : m_xAxisData.topic;
        stream << xLabel << ',' << FilterStore::topicNames(m_seriesMap.keys()).join(',') << Qt::endl;

        // i hate CSV
        QMap<double, QStringList> xyValueMap{};
        for (const QLineSeries *series : m_seriesMap.values()) {
            for (const QPointF &point : series->points()) {
                QStringList list{};
                if (xyValueMap.contains(point.x())) {
                    list = xyValueMap.value(point.x());
                }
                list << QString::number(point.y());
                xyValueMap.insert(point.x(), list);
            }
        }

        QMapIterator iter(xyValueMap);

        while (iter.hasNext()) {
            iter.next();
            stream << iter.key() << ',' << iter.value().join(',') << Qt::endl;
        }

        stream.flush();
        file.close();
    } else {
        QMessageBox::critical(this, "Failed to Open File For Reading", "Failed to open file for reading. Directory may not exist, or you may lack permissions to write here.");
    }
}
