#include "widgets/DoubleDialWidget.h"
#include "stores/TopicStore.h"

#include <QApplication>

DoubleDialWidget::DoubleDialWidget(const QString &topic, const double &defaultValue, const QString &title) : DoubleDisplayWidget(topic, defaultValue, title) {
    m_entry = TopicStore::subscribe(topic.toStdString(), this);

    m_dial = new BetterDial(this);
    m_type = WidgetTypes::DoubleDial;

    m_fakeValue = defaultValue * 100;

    m_dial->setMin(0);
    m_dial->setMax(36000);

    m_layout->removeWidget(m_text);

    m_layout->addWidget(m_dial, 1, 0, 2, 1);
    m_layout->addWidget(m_text, 3, 0);

    connect(m_dial, &BetterDial::sliderMoved, this, [this](int position) {
        m_entry->SetDouble(position / 100.);
        m_text->setText(QString::number(position / 100.));
    });
    m_ready = true;
}

DoubleDialWidget::~DoubleDialWidget() {
    TopicStore::unsubscribe(m_entry, this);
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

void DoubleDialWidget::setValue(const nt::Value &value) {
    if (!m_text->hasFocus()) {
        m_value = value.GetDouble();

        m_fakeValue = m_value * 100;
        setText(QString::number(m_value));

        if (!m_dial->isDragging()) m_dial->setValue(m_fakeValue);
    }
}

void DoubleDialWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
        m_fakeValue = m_value * 100.;

        m_dial->setValue(m_fakeValue);
    }
}
