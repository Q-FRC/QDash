#include "widgets/BooleanCheckboxWidget.h"
#include "stores/TopicStore.h"

BooleanCheckboxWidget::BooleanCheckboxWidget(const QString &title, const bool &defaultValue, const QString &topic) : BaseWidget(WidgetTypes::BooleanCheckbox, title, topic)
{
    m_value = defaultValue;

    m_checkbox = new QCheckBox(this);
    m_checkbox->setObjectName("checkboxWidget");

    m_checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_checkbox->setStyleSheet("QCheckBox::indicator:unchecked { width: 30px; height: 30px; } QCheckBox::indicator:checked { width: 30px; height: 30px; }");

    m_layout->addWidget(m_checkbox, 1, 0, 3, 1, Qt::AlignHCenter);

    connect(m_checkbox, &QCheckBox::stateChanged, this, [this](int state) {
        m_entry->SetBoolean(state == Qt::Checked);
        m_value = state == Qt::Checked;
    });
}

BooleanCheckboxWidget::~BooleanCheckboxWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

int BooleanCheckboxWidget::checkboxSize() {
    return m_checkboxSize;
}

void BooleanCheckboxWidget::setCheckboxSize(int size) {
    m_checkboxSize = size;

    m_checkbox->setStyleSheet(QString("QCheckBox::indicator:unchecked { width: %1px; height: %1px; } QCheckBox::indicator:checked { width: %1px; height: %1px; }").arg(size));
}

QJsonObject BooleanCheckboxWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("value", m_value);
    object.insert("checkboxSize", m_checkboxSize);

    return object;
}

BaseWidget * BooleanCheckboxWidget::fromJson(QJsonObject obj) {
    BooleanCheckboxWidget *widget = new BooleanCheckboxWidget(
        obj.value("title").toString(""),
        obj.value("value").toBool(false),
        obj.value("topic").toString(""));

    widget->setCheckboxSize(obj.value("checkboxSize").toInt(30));
    return widget;
}

void BooleanCheckboxWidget::setValue(nt::Value value) {
    m_value = value.GetBoolean();

    m_checkbox->setCheckState(m_value ? Qt::Checked : Qt::Unchecked);
}
