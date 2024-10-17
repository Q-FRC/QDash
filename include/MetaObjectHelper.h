#ifndef METAOBJECTHELPER_H
#define METAOBJECTHELPER_H

#include <QMetaObject>
#include <QObject>
#include <QQmlProperty>

class MetaObjectHelper : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    using QObject::QObject;
    Q_INVOKABLE QString typeName(QObject* object, const QString& property) const
    {
        QQmlProperty qmlProperty(object, property);
        QMetaProperty metaProperty = qmlProperty.property();
        return metaProperty.typeName();
    }
};

#endif // METAOBJECTHELPER_H

