#include "widgets/SendableFieldWidget.h"

#include "stores/TopicStore.h"

SendableFieldWidget::~SendableFieldWidget() {
    TopicStore::unsubscribe(m_entry, this);
}
