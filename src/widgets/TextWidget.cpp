#include "widgets/TextWidget.h"
#include "stores/TopicStore.h"

#include <QAction>
#include <QMenu>
#include <QFontDialog>

TextWidget::TextWidget(const WidgetTypes &type, const QString &topic, const QString &defaultText, const QString &title) : BaseWidget::BaseWidget(type, title, topic)
{
    m_entry = TopicStore::subscribe(topic.toStdString(), this);
    m_text = new QLineEdit(defaultText, this);

    m_layout->addWidget(m_text, 1, 0, 3, 1);

    m_text->setStyleSheet("border: none; border-bottom: 1px solid white;");
    m_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

TextWidget::~TextWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

QString TextWidget::text() {
    return m_text->text();
}

void TextWidget::setText(const QString &text) {
    m_text->setText(text);
}

QFont TextWidget::font() {
    return m_text->font();
}

void TextWidget::setFont(const QFont &font) {
    m_text->setFont(font);
    m_text->setMinimumWidth(m_text->fontMetrics().maxWidth());
}

QMenu *TextWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = BaseWidget::constructContextMenu(data);

    QAction *textFontAction = new QAction("Text Font", menu);
    menu->addAction(textFontAction);

    connect(textFontAction, &QAction::triggered, this, [this](bool) {
        setFont(QFontDialog::getFont(0, font(), this, "Set Text Font"));
    });

    return menu;
}
