#pragma once

#include "widgets/BaseWidget.h"

#include "misc/FieldImage.h"

class FieldWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(std::vector<double> value MEMBER m_value)
    Q_PROPERTY(Globals::File Image READ image WRITE setImage REQUIRED)
    Q_PROPERTY(double Robot_Width READ robotWidth WRITE setRobotWidth REQUIRED)
    Q_PROPERTY(double Robot_Length READ robotLength WRITE setRobotLength REQUIRED)
    // TODO: possibly different robot shapes? eh not worth it.
protected:
    std::vector<double> m_value{};

    FieldImage *m_imageLabel;
    Globals::File m_image;

    double m_width;
    double m_length;
public:
    FieldWidget(const QString &title, const std::vector<double> &defaultValue, const QString &topic, bool fromSendable = false);
    ~FieldWidget();

    double robotWidth();
    void setRobotWidth(double width);

    double robotLength();
    void setRobotLength(double length);

    Globals::File &image();
    void setImage(Globals::File image);

    void setValue(const nt::Value &value) override;

    QJsonObject saveObject() override;
    static BaseWidget * fromJson(QJsonObject obj);
};
