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
#include "widgets/FieldWidget.h"
#include "widgets/SendableFieldWidget.h"
#include "widgets/CommandWidget.h"
#include "widgets/GraphWidget.h"

#include "dialogs/WidgetDialogGenerator.h"

#include <QMenu>
#include <QFontDialog>
#include <QStyleOption>
#include <QPainter>
#include <QJsonArray>
#include <QApplication>
#include <QMouseEvent>

BaseWidget::BaseWidget(const WidgetTypes &type, const QString &title, const QString &topic)
{
    m_type = type;
    m_topic = topic;

    m_layout = new QGridLayout(this);
    m_title = new QLineEdit(title, this);

    m_layout->addWidget(m_title, 0, 0);

    setStyleSheet("BaseWidget { background-color: " + qApp->palette().color(QPalette::ColorRole::Base).darker(150).name() + "; border: 1px solid white; color: white; }");
    m_title->setStyleSheet("QLineEdit { border: none; border-bottom: 2px solid white; }");

    this->setFrameShape(QFrame::Panel);
    this->setFrameShadow(QFrame::Raised);

    QFont titleFont = this->titleFont();
    titleFont.setBold(true);
    setTitleFont(titleFont);

    m_title->setAlignment(Qt::AlignHCenter);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
    setContentsMargins(5, 5, 5, 5);
}

BaseWidget::~BaseWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

bool BaseWidget::ready() {
    return m_ready;
}

void BaseWidget::setReady(bool ready) {
    m_ready = ready;
    if (ready) emit isReady();
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
    return m_topic;
}

void BaseWidget::setTopic(const QString &topic)
{
    if (m_topic == topic)
        return;

    m_topic = topic;
    if (m_entry != nullptr) TopicStore::unsubscribe(m_topic.toStdString(), this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this);
}

ResizeDirection BaseWidget::resizing() {
    return m_resize;
}

void BaseWidget::setResizing(ResizeDirection direction) {
    m_resize = direction;
}

QMenu *BaseWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = new QMenu(this);

    QAction *resizeAction = new QAction("Resize", menu);

    menu->addAction(resizeAction);

    connect(resizeAction, &QAction::triggered, this, [this, data](bool) {
        WidgetDialogGenerator *dialog = new WidgetDialogGenerator(this, this, true, data);
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

void BaseWidget::setValue(const nt::Value &value) {}

void BaseWidget::forceUpdate() {
    setValue(m_entry->GetValue());
    update();
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

    object.insert("widgetType", (int) m_type);

    int offset = BaseWidget::staticMetaObject.propertyOffset();
    int propertyCount = metaObject()->propertyCount();

    for (int i = offset; i < propertyCount; ++i) {
        QMetaProperty property = metaObject()->property(i);
        QString propertyName(property.name());

        int id = property.typeId();

#define PROPERTY_FUNCTION(type, function) if (id == type) object.insert(propertyName, function(property)); else

        PROPERTY_FUNCTION(QMetaType::Double, writeDoubleProperty)
        PROPERTY_FUNCTION(QMetaType::Int, writeIntProperty)
        PROPERTY_FUNCTION(QMetaType::Bool, writeBoolProperty)
        PROPERTY_FUNCTION(QMetaType::QColor, writeColorProperty)
        PROPERTY_FUNCTION(QMetaType::QVariantMap, writeMapProperty)
        PROPERTY_FUNCTION(QMetaType::QVariantList, writeListProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::File, writeFileProperty)
        PROPERTY_FUNCTION(QMetaType::QFont, writeFontProperty)
        PROPERTY_FUNCTION(QMetaType::QString, writeStringProperty)
        PROPERTY_FUNCTION(QMetaType::QUrl, writeStringProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::FrameShape, writeShapeProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::TopicList, writeTopicListProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::XAxis, writeXAxisProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::TopicColorMap, writeTopicColorMapProperty)
        { // else
            qCritical() << "Bad metatype for property" << property.name() << (QMetaType::Type) id;
            continue;
        } // else

#undef PROPERTY_FUNCTION
    }

    return object;
}

WidgetData BaseWidget::fromJson(QJsonObject obj) {
    int offset = staticMetaObject.propertyOffset();
    int propertyCount = metaObject()->propertyCount();

    for (int i = offset; i < propertyCount; ++i) {
        QMetaProperty property = metaObject()->property(i);
        QString propertyName(property.name());
        if (!obj.contains(propertyName)) {
            continue;
        }

        QJsonValue value = obj.value(propertyName);
        int id = property.typeId();

#define PROPERTY_FUNCTION(type, function) if (id == type) property.write(this, function(property, value)); else

        PROPERTY_FUNCTION(QMetaType::Double, readDoubleProperty)
        PROPERTY_FUNCTION(QMetaType::Int, readIntProperty)
        PROPERTY_FUNCTION(QMetaType::Bool, readBoolProperty)
        PROPERTY_FUNCTION(QMetaType::QColor, readColorProperty)
        PROPERTY_FUNCTION(QMetaType::QVariantMap, readMapProperty)
        PROPERTY_FUNCTION(QMetaType::QVariantList, readListProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::File, readFileProperty)
        PROPERTY_FUNCTION(QMetaType::QFont, readFontProperty)
        PROPERTY_FUNCTION(QMetaType::QString, readStringProperty)
        PROPERTY_FUNCTION(QMetaType::QUrl, readStringProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::FrameShape, readShapeProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::TopicList, readTopicListProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::XAxis, readXAxisProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::TopicColorMap, readTopicColorMapProperty)
        { // else
            qCritical() << "Bad metatype for property" << property.name() << (QMetaType::Type) id;
            continue;
        } // else

#undef PROPERTY_FUNCTION
    }

    QJsonArray geometry = obj.value("geometry").toArray({});

    WidgetData data;
    data.row = geometry.at(0).toInt(0);
    data.col = geometry.at(1).toInt(0);
    data.rowSpan = geometry.at(2).toInt(1);
    data.colSpan = geometry.at(3).toInt(1);

    m_ready = true;

    return data;
}

BaseWidget *BaseWidget::defaultWidgetFromTopic(QString ntTopic, WidgetTypes type) {

    BaseWidget *baseWidget;

#define REGISTER_WIDGET_TYPE(widgetType, widget) if (widgetType == type) { \
    baseWidget = new widget(ntTopic); \
} else

REGISTER_WIDGET_TYPE(WidgetTypes::BooleanCheckbox, BooleanCheckboxWidget)
REGISTER_WIDGET_TYPE(WidgetTypes::BooleanDisplay, BooleanDisplayWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::DoubleDisplay, DoubleDisplayWidget)
REGISTER_WIDGET_TYPE(WidgetTypes::DoubleDial, DoubleDialWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::SendableChooser, StringChooserWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::CameraView, CameraViewWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::EnumWidget, EnumWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::IntegerDisplay, IntegerDisplayWidget)
REGISTER_WIDGET_TYPE(WidgetTypes::IntegerDial, IntegerDialWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::Field, FieldWidget)
REGISTER_WIDGET_TYPE(WidgetTypes::SendableField, SendableFieldWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::Command, CommandWidget)

REGISTER_WIDGET_TYPE(WidgetTypes::Graph, GraphWidget)

// implicit-condition: StringDisplay
{ // else
    baseWidget = new StringDisplayWidget(ntTopic);
} // else

#undef REGISTER_WIDGET_TYPE

return baseWidget;
}

// JSON stuff
QVariant BaseWidget::readDoubleProperty(const QMetaProperty &property, const QJsonValue &value) {
    return value.toDouble(property.read(this).toDouble());
}

QVariant BaseWidget::readIntProperty(const QMetaProperty &property, const QJsonValue &value) {
    return value.toInt(property.read(this).toInt());
}

QVariant BaseWidget::readBoolProperty(const QMetaProperty &property, const QJsonValue &value) {
    return value.toBool(property.read(this).toBool());
}

QVariant BaseWidget::readColorProperty(const QMetaProperty &property, const QJsonValue &value) {
    return QColor(value.toString(property.read(this).value<QColor>().name()));
}

QVariant BaseWidget::readMapProperty(const QMetaProperty &property, const QJsonValue &value) {
    QVariantMap map = value.toObject().toVariantMap();
    return map.empty() ? property.read(this).toMap() : map;
}

QVariant BaseWidget::readListProperty(const QMetaProperty &property, const QJsonValue &value) {
    QVariantList list = value.toArray().toVariantList();
    return list.empty() ? property.read(this).toList() : list;
}

QVariant BaseWidget::readFileProperty(const QMetaProperty &property, const QJsonValue &value) {
    return QVariant::fromValue<Globals::File>(Globals::File{value.toString(property.read(this).value<Globals::File>().fileName)});
}

QVariant BaseWidget::readFontProperty(const QMetaProperty &property, const QJsonValue &value) {
    return QVariant::fromValue<QFont>(QFont(value.toString(property.read(this).value<QFont>().toString())));
}

QVariant BaseWidget::readStringProperty(const QMetaProperty &property, const QJsonValue &value) {
    return value.toString(property.read(this).toString());
}

QVariant BaseWidget::readShapeProperty(const QMetaProperty &property, const QJsonValue &value) {
    return QVariant::fromValue<Globals::FrameShape>(Globals::shapeNameMap.value(
        value.toString(
            Globals::shapeNameMap.key(property.read(this).value<Globals::FrameShape>()))));
}

QVariant BaseWidget::readTopicListProperty(const QMetaProperty &property, const QJsonValue &value) {
    QJsonArray array = value.toArray({});
    QList<Globals::Topic> list{};

    for (QJsonValueRef ref : array) {
        QJsonObject obj = ref.toObject();
        Globals::Topic topic{
            obj.value("name").toString(),
            (TopicTypes) obj.value("type").toInt()
        };

        list.append(topic);
    }

    return QVariant::fromValue<QList<Globals::Topic>>(list);
}

QVariant BaseWidget::readXAxisProperty(const QMetaProperty &property, const QJsonValue &value) {
    QJsonObject obj = value.toObject({});
    Globals::GraphXAxis current = property.read(this).value<Globals::GraphXAxis>();

    return QVariant::fromValue<Globals::GraphXAxis>(
        Globals::GraphXAxis{
            obj.value("useTime").toBool(current.useTime),
                obj.value("topic").toString(current.topic)
        });
}

QVariant BaseWidget::readTopicColorMapProperty(const QMetaProperty &property, const QJsonValue &value) {
    QJsonArray array = value.toArray();

    QHash<Globals::Topic, QColor> map{};

    for (const QJsonValueRef &ref : array) {
        QJsonObject obj = ref.toObject();
        QJsonObject topicObj = obj.value("topic").toObject();
        Globals::Topic topic {
            topicObj.value("name").toString(),
            (TopicTypes) topicObj.value("type").toInt()
        };

        QColor color = obj.value("color").toString("");

        map.insert(topic, color);
    }

    return QVariant::fromValue(map);
}

// Write
QJsonValue BaseWidget::writeDoubleProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).toDouble());
}

QJsonValue BaseWidget::writeIntProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).toInt());
}

QJsonValue BaseWidget::writeBoolProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).toBool());
}

QJsonValue BaseWidget::writeColorProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).value<QColor>().name());
}

QJsonValue BaseWidget::writeMapProperty(const QMetaProperty &property) {
    return QJsonValue(
        QJsonObject::fromVariantMap(
            property.read(this).toMap()));
}

QJsonValue BaseWidget::writeListProperty(const QMetaProperty &property) {
    return QJsonValue(
        QJsonArray::fromVariantList(
            property.read(this).toList()));
}

QJsonValue BaseWidget::writeFileProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).value<Globals::File>().fileName);
}

QJsonValue BaseWidget::writeFontProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).value<QFont>().toString());
}

QJsonValue BaseWidget::writeStringProperty(const QMetaProperty &property) {
    return QJsonValue(property.read(this).toString());
}

QJsonValue BaseWidget::writeShapeProperty(const QMetaProperty &property) {
    return QJsonValue(Globals::shapeNameMap.key(property.read(this).value<Globals::FrameShape>()));
}

QJsonValue BaseWidget::writeTopicListProperty(const QMetaProperty &property) {
    QJsonArray array{};
    QList<Globals::Topic> list = property.read(this).value<QList<Globals::Topic>>();

    for (const Globals::Topic &topic : list) {
        array.append(
            QJsonObject{
                {"name", topic.name},
                {"type", (int) topic.type}
            });
    }

    return array;
}

QJsonValue BaseWidget::writeXAxisProperty(const QMetaProperty &property) {
    Globals::GraphXAxis xAxis = property.read(this).value<Globals::GraphXAxis>();

    return QJsonObject(
        {
            {"useTime", xAxis.useTime},
            {"topic", xAxis.topic}
        }
    );
}

QJsonValue BaseWidget::writeTopicColorMapProperty(const QMetaProperty &property) {
    QJsonArray array{};
    QHash<Globals::Topic, QColor> map = property.read(this).value<QHash<Globals::Topic, QColor>>();

    QHashIterator iter(map);

    while (iter.hasNext()) {
        iter.next();

        Globals::Topic topic = iter.key();
        QColor color = iter.value();

        QJsonObject obj{};
        QJsonObject topicObj{
            {"name", topic.name},
            {"type", (int) topic.type}
        };

        obj.insert("topic", topicObj);
        obj.insert("color", color.name());
        array.append(obj);
    }

    return array;
}

// etc

void BaseWidget::mouseMoveEvent(QMouseEvent *event) {
    QRect rect = this->rect();
    QRect left(rect.topLeft(), rect.bottomLeft());
    QRect right(rect.topRight(), rect.bottomRight());
    QRect top(rect.topRight(), rect.topLeft());
    QRect bottom(rect.bottomRight(), rect.bottomLeft());

    QRect pointerRect = QRect(event->position().toPoint() - QPoint(10, 10), QSize(20, 20));

    QCursor qcursor;
    Qt::CursorShape shape = Qt::ArrowCursor;

    ResizeDirection direction = NONE;

#define ONE_EDGE(edge, edgeDirection) if (edge.intersects(pointerRect)) { direction |= edgeDirection; }

    ONE_EDGE(left, LEFT)
    ONE_EDGE(right, RIGHT)
    ONE_EDGE(top, TOP)
    ONE_EDGE(bottom, BOTTOM)

#undef ONE_EDGE

    switch (direction) {
    case LEFT:
    case RIGHT:
        shape = Qt::SizeHorCursor;
        break;
    case TOP:
    case BOTTOM:
        shape = Qt::SizeVerCursor;
        break;
    case TOP | RIGHT:
    case BOTTOM | LEFT:
        shape = Qt::SizeBDiagCursor;
        break;
    case TOP | LEFT:
    case BOTTOM | RIGHT: {
        shape = Qt::SizeFDiagCursor;
        break;
    }
    default: {}
    }

    if (shape != qcursor.shape()) {
        m_resize = direction;
        qcursor.setShape(shape);
        setCursor(qcursor);
    } else {
        m_resize = NONE;
    }

    event->ignore();
}

void BaseWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    mouseMoveEvent(event);
    event->ignore();
}
