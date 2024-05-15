#include "widgets/IntegerDialWidget.h"
#include "stores/TopicStore.h"

#include <QApplication>
#include <QKeyEvent>

IntegerDialWidget::IntegerDialWidget(const QString &topic, const int &defaultValue, const QString &title) : IntegerDisplayWidget(topic, defaultValue, title, false) {
    setTopic(topic);
    m_dial = new BetterDial(this);
    m_type = WidgetTypes::IntegerDial;

    m_dial->setMin(0);
    m_dial->setMax(1000);

    m_layout->removeWidget(m_text);

    m_layout->addWidget(m_dial, 1, 0, 2, 1);
    m_layout->addWidget(m_text, 3, 0);

    connect(m_dial, &BetterDial::sliderMoved, this, [this](int position) {
        m_entry.SetInteger(position);
        m_value = position;
        m_text->setText(QString::number(position));
    });
    setReady(true);
}

IntegerDialWidget::~IntegerDialWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

int IntegerDialWidget::min() {
    return m_min;
}

void IntegerDialWidget::setMin(int min) {
    m_min = min;
    m_dial->setMin(min);
}

int IntegerDialWidget::max() {
    return m_max;
}

void IntegerDialWidget::setMax(int max) {
    m_max = max;
    m_dial->setMax(max);
}

double IntegerDialWidget::startingAngle() {
    return m_startingAngle;
}

void IntegerDialWidget::setStartingAngle(double angle) {
    m_startingAngle = angle;
    m_dial->setStartingAngle(angle * M_PI / 180.);
}

void IntegerDialWidget::setTopic(const QString &topic) {
    m_topic = topic;
    TopicStore::unsubscribe(m_topic, this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this, NT_INTEGER);
}

void IntegerDialWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (!m_text->hasFocus()) {
        m_value = value.GetInteger();
        setText(QString::number(m_value));

        if (!m_dial->isDragging()) m_dial->setValue(m_value);
    }
}

void IntegerDialWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry.SetInteger(m_text->text().toInt());
        m_value = m_text->text().toInt();

        m_dial->setValue(m_value);
    } else {
        event->ignore();
    }
}
