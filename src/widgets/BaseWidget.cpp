#include "widgets/BaseWidget.h"
#include "Globals.h"
#include "stores/TopicStore.h"

#include "widgets/BooleanCheckboxWidget.h"
#include "widgets/BooleanDisplayWidget.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/DoubleDialWidget.h"
#include "widgets/EnumWidget.h"
#include "widgets/NumberDisplayWidget.h"
#include "widgets/StringChooserWidget.h"
#include "widgets/StringDisplayWidget.h"

#include "misc/WidgetDialogGenerator.h"

#include <QMenu>
#include <QFontDialog>
#include <QStyleOption>
#include <QPainter>
#include <QJsonArray>

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

    BaseWidget *widget;

    switch (widgetType) {
    case WidgetTypes::BooleanCheckbox: {
        BooleanCheckboxWidget *checkboxWidget = new BooleanCheckboxWidget(
            obj.value("title").toString(),
            obj.value("value").toBool(),
            obj.value("topic").toString());

        checkboxWidget->setCheckboxSize(obj.value("checkboxSize").toInt());
        widget = checkboxWidget;
        break;
    }
    case WidgetTypes::BooleanDisplay: {
        BooleanDisplayWidget *displayWidget = new BooleanDisplayWidget(
            obj.value("title").toString(),
            obj.value("value").toBool(),
            obj.value("topic").toString());

        displayWidget->setTrueColor(QColor::fromString(obj.value("trueColor").toString()));
        displayWidget->setFalseColor(QColor::fromString(obj.value("falseColor").toString()));

        widget = displayWidget;
        break;
    }
    case WidgetTypes::DoubleDisplay: {
        NumberDisplayWidget *displayWidget = new NumberDisplayWidget(
            WidgetTypes::DoubleDisplay,
            obj.value("title").toString(),
            obj.value("value").toDouble(),
            obj.value("topic").toString());

        QFont font;
        font.fromString(obj.value("textFont").toString());
        displayWidget->setFont(font);

        widget = displayWidget;
        break;
    }
    case WidgetTypes::DoubleDial: {
        DoubleDialWidget *dialWidget = new DoubleDialWidget(
            obj.value("title").toString(),
            obj.value("value").toDouble(),
            obj.value("topic").toString());

        QFont font;
        font.fromString(obj.value("textFont").toString());
        dialWidget->setFont(font);

        dialWidget->setMin(obj.value("min").toDouble());
            dialWidget->setMax(obj.value("max").toDouble());

        widget = dialWidget;
        break;
    }
    case WidgetTypes::SendableChooser: {
        widget = new StringChooserWidget(
            obj.value("title").toString(),
            "",
            obj.value("topic").toString());

        break;
    }
    case WidgetTypes::CameraView: {
        widget = new CameraViewWidget(
            obj.value("title").toString(),
            QUrl(obj.value("url").toString()),
            "");

        break;
    }
    case WidgetTypes::EnumWidget: {
        EnumWidget *enumWidget = new EnumWidget(
            obj.value("title").toString(),
            obj.value("value").toString(),
            obj.value("topic").toString());

        QVariantMap variantColorMap = obj.value("colors").toObject().toVariantMap();

        enumWidget->setColors(variantColorMap);
        widget = enumWidget;
        break;
    }
    case WidgetTypes::StringDisplay:
    default: {
        StringDisplayWidget *displayWidget = new StringDisplayWidget(
            obj.value("title").toString(),
            obj.value("value").toString(),
            obj.value("topic").toString());

        QFont font;
        font.fromString(obj.value("textFont").toString());
        displayWidget->setFont(font);

        widget = displayWidget;
        break;
    }
    } // switch

    QFont titleFont;
    titleFont.fromString(obj.value("titleFont").toString());
    widget->setTitleFont(titleFont);

    QJsonArray geometry = obj.value("geometry").toArray();
    WidgetData data;
    data.tabIdx = tabIdx;
    data.row = geometry.at(0).toInt();
    data.col = geometry.at(1).toInt();
    data.rowSpan = geometry.at(2).toInt();
    data.colSpan = geometry.at(3).toInt();

    return std::make_pair(widget, data);
}

BaseWidget *BaseWidget::defaultWidgetFromTopic(QString ntTopic, WidgetTypes type) {
    BaseWidget *widget;

    switch (type) {
    case WidgetTypes::BooleanCheckbox: {
        widget = new BooleanCheckboxWidget(
            ntTopic,
            false,
            ntTopic);


        break;
    }
    case WidgetTypes::BooleanDisplay: {
        widget = new BooleanDisplayWidget(
            ntTopic,
            false,
            ntTopic);


        break;
    }
    case WidgetTypes::DoubleDisplay: {
        widget = new NumberDisplayWidget(
            WidgetTypes::DoubleDisplay,
            ntTopic,
            0.,
            ntTopic);


        break;
    }
    case WidgetTypes::DoubleDial: {
        widget = new DoubleDialWidget(
            ntTopic,
            0.,
            ntTopic);


        break;
    }
    case WidgetTypes::SendableChooser: {
        widget = new StringChooserWidget(
            ntTopic,
            "",
            ntTopic);



        break;
    }
    case WidgetTypes::CameraView: {
        widget = new CameraViewWidget(
            "",
            QUrl(),
            "");



        break;
    }
    case WidgetTypes::EnumWidget: {
        widget = new EnumWidget(
            ntTopic,
            "",
            ntTopic);


        break;
    }
    case WidgetTypes::StringDisplay:
    default: {
        widget = new StringDisplayWidget(
            ntTopic,
            "",
            ntTopic);


        break;
    }
    } // switch

    return widget;
}
