#include "widgets/IntegerDialWidget.h"
#include "stores/TopicStore.h"

#include <QApplication>

IntegerDialWidget::IntegerDialWidget(const QString &title, const int &defaultValue, const QString &topic) : IntegerDisplayWidget(title, defaultValue, topic) {
    m_dial = new BetterDial(this);
    m_type = WidgetTypes::IntegerDial;

    m_dial->setMin(0);
    m_dial->setMax(1000);

    m_layout->removeWidget(m_text);

    m_layout->addWidget(m_dial, 1, 0, 2, 1);
    m_layout->addWidget(m_text, 3, 0);

    connect(m_dial, &BetterDial::sliderMoved, this, [this](int position) {
        m_entry->SetInteger(position);
        m_value = position;
        m_text->setText(QString::number(position));
    });
}

IntegerDialWidget::~IntegerDialWidget() {
    TopicStore::unsubscribe(m_entry, this);
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

QJsonObject IntegerDialWidget::saveObject() {
    QJsonObject object = IntegerDisplayWidget::saveObject();

    object.insert("min", min());
    object.insert("max", max());
    object.insert("startingAngle", m_startingAngle);

    return object;
}

void IntegerDialWidget::setValue(const nt::Value &value) {
    if (!m_text->hasFocus()) {
        m_value = value.GetInteger();
        setText(QString::number(m_value));

        if (!m_dial->isDragging()) m_dial->setValue(m_value);
    }
}

BaseWidget * IntegerDialWidget::fromJson(QJsonObject obj) {
    IntegerDialWidget *widget = new IntegerDialWidget(
        obj.value("title").toString(""),
        obj.value("value").toInt(0),
        obj.value("topic").toString(""));

    QFont font;
    font.fromString(obj.value("textFont").toString(qApp->font().toString()));
    widget->setFont(font);

    widget->setMin(obj.value("min").toInt(0));
    widget->setMax(obj.value("max").toInt(1000));

    widget->setStartingAngle(obj.value("startingAngle").toDouble(180.));

    return widget;
}

void IntegerDialWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetInteger(m_text->text().toInt());
        m_value = m_text->text().toInt();

        m_dial->setValue(m_value);
    }
}
