// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TABLISTMODEL_H
#define TABLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonDocument>

#include "Managers/SettingsManager.h"

class TabWidgetsModel;
typedef struct {
    QString title;

    int rows;
    int cols;

    TabWidgetsModel *model;
} Tab;

class TabListModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int selectedTab READ selectedTab NOTIFY selectedTabChanged FINAL)
public:
    enum TLMRoleTypes { TITLE = Qt::UserRole, ROWS, COLS, WIDGETS };

    explicit TabListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    void reset(const QList<Tab> &data);
    Q_INVOKABLE void add(const Tab &t);
    Q_INVOKABLE void add(QString title);

    // Remove data:
    Q_INVOKABLE bool remove(int row, const QModelIndex &parent = QModelIndex());

    static TabListModel *loadObject(QObject *parent, const QJsonArray &arr);

    Q_INVOKABLE QJsonObject saveObject() const;
    // Q_INVOKABLE void load(const QString &fileName = "");

    Q_INVOKABLE void clear();

    int selectedTab() const;
    void selectTab(const QString &tab);

signals:
    void selectedTabChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Tab> m_data;

    int m_selectedTab = 0;
};
#endif // TABLISTMODEL_H
