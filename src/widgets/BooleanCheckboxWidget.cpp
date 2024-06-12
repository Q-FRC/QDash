#include "widgets/BooleanCheckboxWidget.h"
#include "stores/TopicStore.h"

BooleanCheckboxWidget::BooleanCheckboxWidget(const QString &topic, const bool &defaultValue, const QString &title) : BaseWidget(WidgetTypes::BooleanCheckbox, title, topic)
{
    m_entry = TopicStore::subscribe(topic.toStdString(), this);
    m_value = defaultValue;

    m_checkbox = new QCheckBox(this);
    m_checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_checkbox->setStyleSheet("QCheckBox::indicator:unchecked { width: 30px; height: 30px; } QCheckBox::indicator:checked { width: 30px; height: 30px; }");

    m_layout->addWidget(m_checkbox, 1, 0, 3, 1, Qt::AlignHCenter);

    connect(m_checkbox, &QCheckBox::stateChanged, this, [this](int state) {
        m_entry->SetBoolean(state == Qt::Checked);
        m_value = state == Qt::Checked;
    });

    m_ready = true;
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

void BooleanCheckboxWidget::setValue(const nt::Value &value) {
    m_value = value.GetBoolean();

    m_checkbox->setCheckState(m_value ? Qt::Checked : Qt::Unchecked);
}
