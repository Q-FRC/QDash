#include "widgets/DoubleDialWidget.h"
#include "stores/TopicStore.h"

DoubleDialWidget::DoubleDialWidget(const QString &title, const double &defaultValue, const QString &topic) : NumberDisplayWidget(WidgetTypes::DoubleDial, title, defaultValue, topic) {
    m_dial = new QDial(this);

    m_fakeValue = defaultValue * 100;

    m_dial->setMinimum(0);
    m_dial->setMaximum(36000);

    m_layout->removeWidget(m_text);

    m_layout->addWidget(m_dial, 1, 0, 2, 1);
    m_layout->addWidget(m_text, 3, 0);

    connect(m_dial, &QAbstractSlider::sliderMoved, this, [this](int position) {
        m_entry->SetDouble(position / 100.);
        m_text->setText(QString::number(position / 100.));
    });
}

DoubleDialWidget::~DoubleDialWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

double DoubleDialWidget::min() {
    return m_min;
}

void DoubleDialWidget::setMin(double min) {
    m_min = min;
    m_dial->setMinimum(min * 100.);
}

double DoubleDialWidget::max() {
    return m_max;
}

void DoubleDialWidget::setMax(double max) {
    m_max = max;
    m_dial->setMaximum(max * 100.);
}

QJsonObject DoubleDialWidget::saveObject() {
    QJsonObject object = NumberDisplayWidget::saveObject();

    object.insert("min", min());
    object.insert("max", max());
    object.insert("widgetType", (int) WidgetTypes::DoubleDial);

    return object;
}

void DoubleDialWidget::update() {
    if (!m_text->hasFocus()) {
        double value = m_entry->GetDouble(m_value);

        m_value = value;
        m_fakeValue = value * 100;
        setText(QString::number(value));

        m_dial->setValue(m_fakeValue);
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
