// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ACCENTSLISTMODEL_H
#define ACCENTSLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QObject>
#include <QQmlEngine>

typedef struct Accent {
    QString name;
    QString accent;
    QString light;
    QString qml;
} Accent;

class AccentsListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    enum ALMRoleTypes { NAME = Qt::UserRole + 1, ACCENT, LIGHT, QML, IDX };

    explicit AccentsListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void loadObject(const QJsonDocument &doc);
    Q_INVOKABLE void load();

    Q_INVOKABLE QString accent(const QString &name);
    Q_INVOKABLE QString light(const QString &name);
    Q_INVOKABLE QString qml(const QString &name);

    Q_INVOKABLE QStringList names() const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Accent> m_data;
};

#endif // ACCENTSLISTMODEL_H
