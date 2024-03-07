#include "stores/TopicStore.h"

#include "qtimer.h"
#include "widgets/BaseWidget.h"

#include "Globals.h"

QList<Listener> TopicStore::Listeners{};
QHash<std::string, nt::NetworkTableEntry *> TopicStore::topicEntryMap{};

TopicStore::TopicStore()
{
    throw std::runtime_error("TopicStore is a static class.");
}

bool TopicStore::hasEntry(std::string topic) {
    for (const Listener &listener : Listeners) {
        if (topic == listener.topic) {
            return true;
        }
    }
    return false;
}

bool TopicStore::hasEntry(QString topic) {
    return hasEntry(topic.toStdString());
}

bool TopicStore::widgetSubscribed(std::string topic, BaseWidget *subscriber) {
    return !getEntry(topic, subscriber).isNull;
}

bool TopicStore::widgetSubscribed(QString topic, BaseWidget *subscriber) {
    return !getEntry(topic, subscriber).isNull;
}

Listener TopicStore::getEntry(std::string topic, BaseWidget *subscriber) {
    for (Listener listener : Listeners) {
        if (listener.topic == topic && listener.subscriber == subscriber) {
            return listener;
        }
    }

    Listener l;
    l.isNull = true;
    return l;
}

Listener TopicStore::getEntry(QString topic, BaseWidget *subscriber) {
    return getEntry(topic.toStdString(), subscriber);
}

bool Listener::operator==(const Listener &other) const {
    return (other.topic == this->topic) &&
           (other.entry == this->entry) &&
           (other.label == this->label) &&
           (other.subscriber == this->subscriber) &&
           (other.listenerHandle == this->listenerHandle) &&
           (other.desiredType == this->desiredType) &&
           (other.isNull == this->isNull);
}

nt::NetworkTableEntry *TopicStore::subscribe(std::string ntTopic, BaseWidget *subscriber, NT_Type desiredType, QString label, bool writeOnly) {
    Listener listener;
    nt::NetworkTableEntry *entry = nullptr;
    if (!hasEntry(ntTopic)) {
        entry = new nt::NetworkTableEntry(nt::GetEntry(Globals::inst.GetHandle(), ntTopic));

        topicEntryMap.insert(ntTopic, entry);
    }

    if (entry == nullptr) entry = topicEntryMap.value(ntTopic);

    listener = {
        ntTopic,
        label,
        entry,
        subscriber,
        0,
        nt::ListenerCallback(),
        desiredType,
        false
    };

    if (!writeOnly) {
        nt::ListenerCallback updateWidget = [entry, subscriber, desiredType, label, ntTopic](const nt::Event &event = nt::Event()) {
            if (!subscriber || !entry) {
                return;
            }

            if (event.Is(nt::EventFlags::kPublish)) {
                QMetaObject::invokeMethod(subscriber, [subscriber] {
                    subscriber->reconnect();
                });
                return;
            }

            nt::Value value;
            if (!event.Is(nt::EventFlags::kValueAll)) {
                value = entry->GetValue();
            } else {
                value = event.GetValueEventData()->value;
            }

            // ensure thread-safety
            // this is mild anal cancer
            if (value.IsValid()) {
                QMetaObject::invokeMethod(subscriber, [subscriber, value, label, desiredType, ntTopic] {
                    if (value.type() != desiredType) {
                        qCritical() << "Type for topic" <<
                            QString::fromStdString(ntTopic) <<
                            "for widget" <<
                            subscriber->title() <<
                            "is of incorrect type" <<
                            (int) value.type() <<
                            "; expected" <<
                            (int) desiredType;
                        return;
                    }

                    if (subscriber->ready()) {
                        if (subscriber->isEnabled()) {
                            subscriber->setValue(value, label);
                            subscriber->update();
                        }
                    } else {
                        connect(subscriber, &BaseWidget::isReady, subscriber, [subscriber, value, label] {
                                subscriber->setValue(value, label);
                                subscriber->update();
                            }, Qt::SingleShotConnection);
                    }
                }); // QMetaObject and its consequences have been a disaster for the human race
            }
        };

        NT_Listener handle = Globals::inst.AddListener(Globals::inst.GetEntry(ntTopic), nt::EventFlags::kValueAll | nt::EventFlags::kPublish, updateWidget);

        listener.listenerHandle = handle;
        listener.callback = updateWidget;

        QTimer::singleShot(1000, subscriber, [updateWidget] {
            updateWidget(nt::Event());
        });
    }

    Listeners.append(listener);

    return entry;
}

void TopicStore::unsubscribe(std::string ntTopic, BaseWidget *subscriber) {
    if (!hasEntry(ntTopic) || !widgetSubscribed(ntTopic, subscriber)) return;

    Listener listener = getEntry(ntTopic, subscriber);
    Listeners.removeOne(listener);

    if (!hasEntry(ntTopic)) {
        if (listener.entry) listener.entry->Unpublish();
        topicEntryMap.remove(ntTopic);
    }

    Globals::inst.RemoveListener(listener.listenerHandle);
}

void TopicStore::unsubscribe(QString ntTopic, BaseWidget *subscriber) {
    unsubscribe(ntTopic.toStdString(), subscriber);
}

void TopicStore::unsubscribe(nt::NetworkTableEntry *entry, BaseWidget *subscriber) {
    unsubscribe(entry->GetName(), subscriber);
}

double TopicStore::getDoubleFromEntry(nt::NetworkTableEntry *entry) {
    nt::Value value = entry->GetValue();

    if (value.IsBoolean()) {
        return (double) entry->GetBoolean(0);
    } else if (value.IsDouble()) {
        return entry->GetDouble(0.);
    } else if (value.IsInteger()) {
        return (double) entry->GetInteger(0);
    }

    return 0.;
}

void TopicStore::updateTopic(std::string topic, BaseWidget *subscriber, QString label) {
    Listener l = getEntry(topic, subscriber);
    if (l.isNull) return;

    nt::ListenerCallback callback = l.callback;
    callback(nt::Event());
}
