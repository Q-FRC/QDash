#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "widgets/BaseWidget.h"

#include <QLineSeries>
#include <QChartView>
#include <QChart>
#include <QTimer>
#include <QElapsedTimer>
#include <QMenu>

#include <QValueAxis>

class ChartView;

class GraphWidget : public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QHash<Globals::Topic, QColor> Topics READ topics WRITE setTopics REQUIRED)
    Q_PROPERTY(double Update_Frequency_Seconds READ updateFrequency WRITE setUpdateFrequency REQUIRED)
    Q_PROPERTY(double Time_Scale_Seconds READ maxTimeScale WRITE setMaxTimeScale REQUIRED)
    Q_PROPERTY(double Max_Y_Value READ maxYValue WRITE setMaxYValue REQUIRED)
    Q_PROPERTY(double Min_Y_Value READ minYValue WRITE setMinYValue REQUIRED)
    Q_PROPERTY(Globals::GraphXAxis X_Axis_Topic READ xAxisData WRITE setXAxisData REQUIRED)
private:
    QHash<Globals::Topic, QColor> m_topics{};

    QHash<Globals::Topic, nt::NetworkTableEntry *> m_entryMap;
    QHash<Globals::Topic, QLineSeries *> m_seriesMap;

    QTimer *m_timer;
    QElapsedTimer m_elapsed;
    ChartView *m_chart;

    QValueAxis *m_xAxis;
    QValueAxis *m_yAxis;

    double m_maxTimeScale = 0.;
    double m_updateFrequency = 0.;

    double m_maxYValue;
    double m_minYValue;

    double m_maxXValue;
    double m_minXValue;

    Globals::GraphXAxis m_xAxisData{true, ""};

    nt::NetworkTableEntry *m_xAxisEntry;


    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    GraphWidget(const QString &topic = "", const QString &title = "");

    QHash<Globals::Topic, QColor> topics();
    void setTopics(QHash<Globals::Topic, QColor> topics);

    double maxTimeScale();
    void setMaxTimeScale(double maxTimeScale);

    double updateFrequency();
    void setUpdateFrequency(double frequency);

    double maxYValue();
    void setMaxYValue(double max);

    double minYValue();
    void setMinYValue(double min);

    Globals::GraphXAxis xAxisData();
    void setXAxisData(const Globals::GraphXAxis &axis);

    double getCurrentXAxis();

    void setValue(const nt::Value &value) override;

    QMenu *constructContextMenu(WidgetData data) override;

public slots:
    void updateGraph();

    void exportCSVPopup();
    void exportCSV(const QString &fileName);
};

#endif // GRAPHWIDGET_H
