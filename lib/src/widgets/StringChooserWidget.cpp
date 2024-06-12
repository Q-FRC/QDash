#include "widgets/StringChooserWidget.h"
#include "Globals.h"
#include "qtimer.h"
#include "stores/TopicStore.h"

#include <QApplication>

StringChooserWidget::StringChooserWidget(const QString &topic, const QString &defaultValue, const QString &title) : BaseWidget(WidgetTypes::SendableChooser, title, topic, true)
{
    setTopic(topic);

    m_chooser = new QComboBox(this);
    m_layout->addWidget(m_chooser, 1, 0);

    connect(m_chooser, &QComboBox::currentTextChanged, this, &StringChooserWidget::updateSelected);

    m_layout->setColumnStretch(0, -1);
    setReady(true);
}

StringChooserWidget::~StringChooserWidget() {
    TopicStore::unsubscribe(m_active, this);
    TopicStore::unsubscribe(m_default, this);
    TopicStore::unsubscribe(m_choices, this);
    TopicStore::unsubscribe(m_selected, this);
}

void StringChooserWidget::setTopic(const QString &topic) {
    m_topic = topic;

    TopicStore::unsubscribe(topic.toStdString() + "/active", this);
    TopicStore::unsubscribe(topic.toStdString() + "/default", this);
    TopicStore::unsubscribe(topic.toStdString() + "/options", this);
    TopicStore::unsubscribe(topic.toStdString() + "/selected", this);

    m_active = TopicStore::subscribe(topic.toStdString() + "/active", this, NT_STRING, "Active");
    m_default = TopicStore::subscribe(topic.toStdString() + "/default", this, NT_STRING, "Default", true);
    m_choices = TopicStore::subscribe(topic.toStdString() + "/options", this, NT_STRING_ARRAY, "Choices");
    m_selected = TopicStore::subscribe(topic.toStdString() + "/selected", this, NT_STRING, "Selected", true);
}

void StringChooserWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (force) {
        qDebug() << "Bro";
        QMap<std::string, QString> map{};
        map.insert("/active", "Active");
        map.insert("/options", "Choices");

        QMapIterator iter(map);
        while (iter.hasNext()) {
            iter.next();
            TopicStore::updateTopic(m_topic.toStdString() + iter.key(), this, iter.value());
        }

        return;
    }

    else {
        if (label == "Active") {
            if (!m_readyToUpdate) {
                m_readyToUpdate = true;
                return;
            }

            QString activeValue = m_chooser->currentText();
            std::string activeValueStd = activeValue.toStdString();

            m_chooser->setCurrentText(QString::fromStdString(std::string{value.GetString()}));

            m_lastSelected = m_chooser->currentText();
        }

        // this is an interesting way to do things
        else if (label == "Choices") {
            QString selected = m_lastSelected;
            std::span<const std::string> choices = value.GetStringArray();

            m_chooser->clear();

            QStringList qchoices{};

            for (const std::string &choice : choices) {
                qchoices << QString::fromStdString(choice);
            }

            qchoices.sort();
            m_chooser->addItems(qchoices);

            updateSelected(selected);
        }
    }
}

void StringChooserWidget::updateSelected(const QString text) {
    if (text.isEmpty() || text.isNull()) return;
    m_selected.SetString(text.toStdString());

    m_lastSelected = text;

    m_chooser->setCurrentText(text);

    QTimer *timer = new QTimer;
    timer->callOnTimeout([this, timer] {
        if (m_active.GetString(m_lastSelected.toStdString()) != m_lastSelected.toStdString()) {
            if (m_flashCounter == 0) {
                setStyleSheet("BaseWidget { background-color: red; }");
            }

            if (m_flashCounter == 5) {
                setStyleSheet("BaseWidget { background-color: " + qApp->palette().color(QPalette::ColorRole::Base).darker(150).name() + "; border: 1px solid white; color: white; }");
            }

            if (m_flashCounter == 10) {
                m_flashCounter = -1;
            }

            ++m_flashCounter;
        } else {
            setStyleSheet("BaseWidget { background-color: " + qApp->palette().color(QPalette::ColorRole::Base).darker(150).name() + "; border: 1px solid white; color: white; }");

            timer->stop();
            timer->deleteLater();
            m_flashCounter = 0;
        }
    });

    timer->start(100);
}

void StringChooserWidget::setConnected(bool connected) {
    BaseWidget::setConnected(connected);
    m_connected = connected;

    if (connected) {
        m_readyToUpdate = false;

        updateSelected(m_lastSelected);
    }
}
