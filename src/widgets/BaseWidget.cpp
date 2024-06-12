#include "widgets/BaseWidget.h"
#include "Globals.h"

#include <QMenu>

BaseWidget::BaseWidget(const QString &title, const QString &topic)
{
    m_layout = new QGridLayout(this);
    m_title = new QLineEdit(title, this);
    m_entry = Globals::inst.GetEntry(topic.toStdString());

    m_layout->addWidget(m_title, 0, 0);

    setStyleSheet("background-color: black; border: 1px solid white; color: white;");
    m_title->setStyleSheet("border: none; border-bottom: 2px solid white;");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QFont BaseWidget::titleFont()
{
    return m_title->font();
}

void BaseWidget::setTitleFont(const QFont &font)
{
    m_title->setFont(font);
}

BaseWidget::~BaseWidget() {}

QString BaseWidget::title()
{
    return m_title->text();
}

void BaseWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

std::string BaseWidget::topic()
{
    return m_entry.GetName();
}

void BaseWidget::setTopic(const std::string &topic)
{
    m_entry.Unpublish();
    m_entry = Globals::inst.GetEntry(topic);
}
