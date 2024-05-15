#include "widgets/DoubleDialWidget.h"
#include "stores/TopicStore.h"

#include <QApplication>
#include <QKeyEvent>

DoubleDialWidget::DoubleDialWidget(const QString &topic, const double &defaultValue, const QString &title) : DoubleDisplayWidget(topic, defaultValue, title, false) {
    setTopic(topic);

    m_dial = new BetterDial(this);
    m_type = WidgetTypes::DoubleDial;

    m_fakeValue = defaultValue * 100;

    m_dial->setMin(0);
    m_dial->setMax(36000);

    m_layout->removeWidget(m_text);

    m_layout->addWidget(m_dial, 1, 0, 2, 1);
    m_layout->addWidget(m_text, 3, 0);

    connect(m_dial, &BetterDial::sliderMoved, this, [this](int position) {
        m_entry.SetDouble(position / 100.);
        m_text->setText(QString::number(position / 100.));
    });
    setReady(true);
}

DoubleDialWidget::~DoubleDialWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

double DoubleDialWidget::min() {
    return m_min;
}

void DoubleDialWidget::setMin(double min) {
    m_min = min;
    m_dial->setMin(min * 100.);
}

double DoubleDialWidget::max() {
    return m_max;
}

void DoubleDialWidget::setMax(double max) {
    m_max = max;
    m_dial->setMax(max * 100.);
}

double DoubleDialWidget::startingAngle() {
    return m_startingAngle;
}

void DoubleDialWidget::setStartingAngle(double angle) {
    m_startingAngle = angle;
    m_dial->setStartingAngle(angle * M_PI / 180.);
}

void DoubleDialWidget::setTopic(const QString &topic) {
    m_topic = topic;
    TopicStore::unsubscribe(m_topic, this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this, NT_DOUBLE);
}

void DoubleDialWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (!m_text->hasFocus()) {
        m_value = value.GetDouble();

        m_fakeValue = m_value * 100;
        setText(QString::number(m_value));

        if (m_dial == nullptr) return;
        if (!m_dial->isDragging()) m_dial->setValue(m_fakeValue);
    }
}

void DoubleDialWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry.SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
        m_fakeValue = m_value * 100.;

        m_dial->setValue(m_fakeValue);
    } else {
        event->ignore();
    }
}
