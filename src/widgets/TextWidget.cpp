#include "widgets/TextWidget.h"

TextWidget::TextWidget(const QString &title, const QString &defaultText, const QString &topic) : BaseWidget::BaseWidget(title, topic)
{
    m_text = new QLineEdit(defaultText, this);

    m_layout->addWidget(m_title, 0, 0, Qt::AlignHCenter | Qt::AlignCenter);
    m_layout->addWidget(m_text, 1, 0, Qt::AlignHCenter | Qt::AlignCenter);
}

TextWidget::~TextWidget() {}

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
}
