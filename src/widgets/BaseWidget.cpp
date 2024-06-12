#include "widgets/BaseWidget.h"
#include "Globals.h"

#include <QMenu>

BaseWidget::BaseWidget(const QString &title, const QString &topic)
{
    m_layout = new QGridLayout(this);
    m_title = new QLabel(title, this);
    m_entry = Globals::inst.GetEntry(topic.toStdString());

    setStyleSheet("background-color: white; border: 2px solid yellow; color: black;");
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
