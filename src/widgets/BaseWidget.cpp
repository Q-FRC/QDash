#include "widgets/BaseWidget.h"
#include "Globals.h"

#include "misc/ResizeDialog.h"

#include <QMenu>
#include <QFontDialog>
#include <QStyleOption>
#include <QPainter>

BaseWidget::BaseWidget(const QString &title, const QString &topic)
{
    m_layout = new QGridLayout(this);
    m_title = new QLineEdit(title, this);
    m_entry = Globals::inst.GetEntry(topic.toStdString());

    m_layout->addWidget(m_title, 0, 0);

    setStyleSheet("BaseWidget { background-color: black; border: 1px solid white; color: white; }");
    m_title->setStyleSheet("QLineEdit { border: none; border-bottom: 2px solid white; }");

    QFont titleFont = this->titleFont();
    titleFont.setBold(true);
    setTitleFont(titleFont);

    m_title->setAlignment(Qt::AlignHCenter);

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

QMenu *BaseWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = new QMenu(this);

    QAction *resizeAction = new QAction("Resize", menu);

    menu->addAction(resizeAction);

    connect(resizeAction, &QAction::triggered, this, [this, data](bool) {
        ResizeDialog *dialog = new ResizeDialog(data);
        dialog->show();

        connect(dialog, &ResizeDialog::finished, this, [this](WidgetData data) {
            emit resizeRequested(data);
        });
    });

    QAction *deleteAction = new QAction("Delete Widget", menu);
    menu->addAction(deleteAction);

    connect(deleteAction, &QAction::triggered, this, [this](bool) {
        emit deleteRequested();
    });

    QAction *titleFontAction = new QAction("Title Font", menu);
    menu->addAction(titleFontAction);

    connect(titleFontAction, &QAction::triggered, this, [this](bool) {
        setTitleFont(QFontDialog::getFont(0, titleFont(), this, "Set Title Font"));
    });

    return menu;
}

void BaseWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
