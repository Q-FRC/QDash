#include "widgets/SendableFieldWidget.h"

#include "widgets/FieldWidget.h"

#include <QJsonArray>

BaseWidget *SendableFieldWidget::fromJson(QJsonObject obj) {
    QJsonArray array = obj.value("value").toArray({});
    std::vector<double> value;

    for (QJsonValueRef ref : array) {
        value.push_back(ref.toDouble(0.));
    }

    FieldWidget *widget = new FieldWidget(
        obj.value("title").toString(""),
        value,
        obj.value("topic").toString(""),
        true);

    widget->setRobotWidth(obj.value("robotWidth").toDouble(0.));
    widget->setRobotLength(obj.value("robotLength").toDouble(0.));
    widget->setImage(Globals::File{obj.value("image").toString(":/2023Field.png")});

    return widget;
}
