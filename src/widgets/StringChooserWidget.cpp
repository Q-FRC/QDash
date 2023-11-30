#include "widgets/StringChooserWidget.h"
#include "Globals.h"
#include "stores/TopicStore.h"

StringChooserWidget::StringChooserWidget(const QString &title, const QString &defaultValue, const QString &topic) : BaseWidget(WidgetTypes::SendableChooser, title, topic),
    m_active(TopicStore::subscribe(topic.toStdString() + "/active", this)),
    m_default(TopicStore::subscribe(topic.toStdString() + "/default", this)),
    m_choices(TopicStore::subscribe(topic.toStdString() + "/options", this)),
    m_selected(TopicStore::subscribe(topic.toStdString() + "/selected", this))
{
    m_value = QString::fromStdString(defaultValue.isEmpty() ? m_active->GetString("") : defaultValue.toStdString());

    m_chooser = new QComboBox(this);

    std::vector<std::string> choices = m_choices->GetStringArray({});
    QStringList qchoices{};

    for (const std::string &choice : choices) {
        qchoices << QString::fromStdString(choice);
    }
    m_chooser->addItems(qchoices);
    m_chooser->setCurrentText(m_value);

    connect(m_chooser, &QComboBox::currentTextChanged, this, [this](QString text) {
        m_selected->SetString(text.toStdString());
    });

    m_layout->addWidget(m_chooser, 1, 0);

    m_layout->setColumnStretch(0, -1);
    m_ready = true;
}

StringChooserWidget::~StringChooserWidget() {
    TopicStore::unsubscribe(m_entry, this);
    TopicStore::unsubscribe(m_active, this);
    TopicStore::unsubscribe(m_default, this);
    TopicStore::unsubscribe(m_choices, this);
    TopicStore::unsubscribe(m_selected, this);
}

QJsonObject StringChooserWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    return object;
}

BaseWidget * StringChooserWidget::fromJson(QJsonObject obj) {
    return new StringChooserWidget(
        obj.value("title").toString(""),
        "",
        obj.value("topic").toString(""));
}

void StringChooserWidget::setValue(const nt::Value &value) {
    QString activeValue = m_chooser->currentText();
    std::string activeValueStd = activeValue.toStdString();

    if (m_active->GetString(activeValueStd) != activeValueStd) {
        if (m_flashCounter == 5) {
            setStyleSheet("background-color: red;");
        }

        if (m_flashCounter == 10) {
            setStyleSheet("BaseWidget { background-color: black; border: 1px solid white; color: white; }");
            m_flashCounter = -1;
        }

        ++m_flashCounter;
    } else {
        setStyleSheet("BaseWidget { background-color: black; border: 1px solid white; color: white; }");
    }

    // this is an interesting way to do things
    std::vector<std::string> choices = m_choices->GetStringArray({});

    std::vector<std::string> currentChoices{};

    for (int i = 0; i < m_chooser->count(); ++i) {
        currentChoices.push_back(m_chooser->itemText(i).toStdString());
    }

    std::sort(choices.begin(), choices.end());
    std::sort(currentChoices.begin(), currentChoices.end());

    if (choices != currentChoices) {
        m_chooser->clear();

        QStringList qchoices{};

        for (const std::string &choice : choices) {
            qchoices << QString::fromStdString(choice);
        }
        m_chooser->addItems(qchoices);
        m_chooser->setCurrentText(m_value);
    }
}
