#include "widgets/BooleanCheckboxWidget.h"

BooleanCheckboxWidget::BooleanCheckboxWidget(const QString &title, const bool &defaultValue, const QString &topic) : BaseWidget(title, topic)
{
    m_value = defaultValue;

    m_checkbox = new QCheckBox(this);
    m_checkbox->setObjectName("checkboxWidget");

    m_checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_checkbox->setStyleSheet("QCheckBox::indicator:unchecked { width: 30px; height: 30px; } QCheckBox::indicator:checked { width: 30px; height: 30px; }");

    m_layout->addWidget(m_checkbox, 1, 0, Qt::AlignHCenter);

    connect(m_checkbox, &QCheckBox::stateChanged, this, [this](int state) {
        m_entry.SetBoolean(state == Qt::Checked);
    });
}

BooleanCheckboxWidget::~BooleanCheckboxWidget() {
    m_entry.Unpublish();
}

QJsonObject BooleanCheckboxWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("value", m_value);
    object.insert("widgetType", (int) WidgetTypes::BooleanCheckbox);

    return object;
}

void BooleanCheckboxWidget::update() {
    bool value = m_entry.GetBoolean(m_value);

    m_value = value;

    m_checkbox->setCheckState(value ? Qt::Checked : Qt::Unchecked);
}
