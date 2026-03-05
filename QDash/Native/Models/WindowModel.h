// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRect>

#include "Managers/SettingsManager.h"

class TabListModel;
typedef struct {
    [[maybe_unused]] QString title; // UNUSED??????

    QRect geometry;

    TabListModel *model;
} Window;

class WindowModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum WMRoleTypes { TITLE = Qt::UserRole, GEOMETRY, TABS };

    explicit WindowModel(Logger *logs, SettingsManager *settings = nullptr,
                         QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    Q_INVOKABLE void add(const QString &title);
    Q_INVOKABLE void add(const Window &w);

    // Remove data:
    Q_INVOKABLE bool remove(int row, const QModelIndex &parent = QModelIndex());

    Q_INVOKABLE void save(const QString &filename = "");
    Q_INVOKABLE QJsonDocument saveObject() const;
    Q_INVOKABLE void load(const QString &filename = "");

    Q_INVOKABLE void clear();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Window> m_data;

    SettingsManager *m_settings;
    Logger *m_logs;
};
