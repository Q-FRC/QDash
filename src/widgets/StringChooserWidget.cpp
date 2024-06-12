#include "widgets/StringChooserWidget.h"
#include "Globals.h"

StringChooserWidget::StringChooserWidget(const QString &title, const QString &topic) : BaseWidget(title, topic)
{
    m_table = Globals::inst.GetTable(topic.toStdString());
    m_active = m_table->GetEntry("active");
    m_default = m_table->GetEntry("default");
    m_choices = m_table->GetEntry("options");

    m_value = QString::fromStdString(m_default.GetString(""));

    m_chooser = new QComboBox(this);

    std::vector<std::string> choices = m_choices.GetStringArray({});
    QStringList qchoices{};

    for (const std::string &choice : choices) {
        qchoices << QString::fromStdString(choice);
    }
    m_chooser->addItems(qchoices);
    m_chooser->setCurrentText(m_value);

    connect(m_chooser, &QComboBox::currentTextChanged, this, [this](QString text) {
        m_active.SetString(text.toStdString());
    });

    m_layout->addWidget(m_chooser, 1, 0);

    m_layout->setColumnStretch(0, -1);
}

StringChooserWidget::~StringChooserWidget() {
    m_entry.Unpublish();
}

QJsonObject StringChooserWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("widgetType", (int) WidgetTypes::SendableChooser);

    return object;
}

// TODO: implement a check/x-mark to verify if values  match
// TODO: allow updating stuff
//void StringChooserWidget::update() {
//    if (false) {
//        m_chooser->clear();

//        m_value = QString::fromStdString(m_active.GetString(""));

//        std::vector<std::string> choices = m_choices.GetStringArray({});
//        QStringList qchoices{};

//        for (const std::string &choice : choices) {
//            qchoices << QString::fromStdString(choice);
//        }
//        m_chooser->addItems(qchoices);
//        m_chooser->setCurrentText(m_value);
//    }
//}
