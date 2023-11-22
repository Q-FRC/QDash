#include "widgets/BaseWidget.h"
#include "Globals.h"
#include "stores/TopicStore.h"

#include "widgets/BooleanCheckboxWidget.h"
#include "widgets/BooleanDisplayWidget.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/DoubleDialWidget.h"
#include "widgets/EnumWidget.h"
#include "widgets/DoubleDisplayWidget.h"
#include "widgets/StringChooserWidget.h"
#include "widgets/StringDisplayWidget.h"
#include "widgets/IntegerDisplayWidget.h"
#include "widgets/IntegerDialWidget.h"

#include "misc/WidgetDialogGenerator.h"

#include <QMenu>
#include <QFontDialog>
#include <QStyleOption>
#include <QPainter>
#include <QJsonArray>
#include <QApplication>

BaseWidget::BaseWidget(const WidgetTypes &type, const QString &title, const QString &topic)
    : m_entry(TopicStore::subscribe(topic.toStdString(), this))
{
    m_type = type;

    m_layout = new QGridLayout(this);
    m_title = new QLineEdit(title, this);

    m_layout->addWidget(m_title, 0, 0);

    setStyleSheet("BaseWidget { background-color: black; border: 1px solid white; color: white; }");
    m_title->setStyleSheet("QLineEdit { border: none; border-bottom: 2px solid white; }");

    QFont titleFont = this->titleFont();
    titleFont.setBold(true);
    setTitleFont(titleFont);

    m_title->setAlignment(Qt::AlignHCenter);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

BaseWidget::~BaseWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

QFont BaseWidget::titleFont()
{
    return m_title->font();
}

void BaseWidget::setTitleFont(const QFont &font)
{
    m_title->setFont(font);
}

QString BaseWidget::title()
{
    return m_title->text();
}

void BaseWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

QString BaseWidget::topic()
{
    return QString::fromStdString(m_entry->GetName());
}

void BaseWidget::setTopic(const QString &topic)
{
    TopicStore::unsubscribe(m_entry, this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this);
}

QMenu *BaseWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = new QMenu(this);

    QAction *resizeAction = new QAction("Resize", menu);

    menu->addAction(resizeAction);

    connect(resizeAction, &QAction::triggered, this, [this, data](bool) {
        WidgetDialogGenerator *dialog = new WidgetDialogGenerator(this, true, data);
        dialog->setWindowTitle("Resize Widget");
        dialog->show();

        connect(dialog, &WidgetDialogGenerator::widgetReady, this, [this](BaseWidget *widget, WidgetData data) {
            emit reconfigRequested(widget, data);
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

QJsonObject BaseWidget::saveObject() {
    QJsonObject object{};

    object.insert("title", title());
    object.insert("topic", QString::fromStdString(m_entry->GetName()));
    object.insert("titleFont", titleFont().toString());
    object.insert("widgetType", (int) m_type);

    return object;
}

std::pair<BaseWidget *, WidgetData> BaseWidget::fromJson(QJsonObject obj, int tabIdx) {
    WidgetTypes widgetType = (WidgetTypes) obj.value("widgetType").toInt();

    BaseWidget *baseWidget;

#define REGISTER_WIDGET_TYPE(type, widget) if (widgetType == type) baseWidget = widget::fromJson(obj); else

    REGISTER_WIDGET_TYPE(WidgetTypes::BooleanCheckbox, BooleanCheckboxWidget)
    REGISTER_WIDGET_TYPE(WidgetTypes::BooleanDisplay, BooleanDisplayWidget)

    REGISTER_WIDGET_TYPE(WidgetTypes::DoubleDisplay, DoubleDisplayWidget)
    REGISTER_WIDGET_TYPE(WidgetTypes::DoubleDial, DoubleDialWidget)

    REGISTER_WIDGET_TYPE(WidgetTypes::SendableChooser, StringChooserWidget)

    REGISTER_WIDGET_TYPE(WidgetTypes::CameraView, CameraViewWidget)

    REGISTER_WIDGET_TYPE(WidgetTypes::EnumWidget, EnumWidget)

    REGISTER_WIDGET_TYPE(WidgetTypes::IntegerDisplay, IntegerDisplayWidget)
    REGISTER_WIDGET_TYPE(WidgetTypes::IntegerDial, IntegerDialWidget)

    // implicit-condition: StringDisplay
    { // else
        baseWidget = StringDisplayWidget::fromJson(obj);
    } // else

#undef REGISTER_WIDGET_TYPE

    QFont titleFont;
    titleFont.fromString(obj.value("titleFont").toString(qApp->font().toString()));
    baseWidget->setTitleFont(titleFont);

    QJsonArray geometry = obj.value("geometry").toArray({});
    WidgetData data;
    data.tabIdx = tabIdx;
    data.row = geometry.at(0).toInt(0);
    data.col = geometry.at(1).toInt(0);
    data.rowSpan = geometry.at(2).toInt(1);
    data.colSpan = geometry.at(3).toInt(1);

    return std::make_pair(baseWidget, data);
}

BaseWidget *BaseWidget::defaultWidgetFromTopic(QString ntTopic, WidgetTypes type) {
    // construct a barebones JSON object
    // and use the existing architecture to make a "default" widget
    QJsonObject obj{};
    obj.insert("topic", ntTopic);
    obj.insert("widgetType", (int) type);

    auto widget = BaseWidget::fromJson(obj, 0);
    return widget.first;
}
