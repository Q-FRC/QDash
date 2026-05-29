// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef REMOTELAYOUTMODEL_H
#define REMOTELAYOUTMODEL_H

#include <QAbstractListModel>
#include <QNetworkAccessManager>

class TopicStore;

typedef struct RemoteLayout {
    QUrl url;
    QString name;
} RemoteLayout;

class RemoteLayoutModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit RemoteLayoutModel(TopicStore *store, QObject *parent = nullptr);
    enum RLMRoleTypes { URL = Qt::UserRole + 1, NAME, IDX };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void add(const QUrl &url, const QString &name);
    Q_INVOKABLE bool load();
    Q_INVOKABLE void clear();
    Q_INVOKABLE void download(const QUrl &url, const QString &filename);

    Q_INVOKABLE QString name(int index);
    Q_INVOKABLE QUrl url(int index);

protected:
    QHash<int, QByteArray> roleNames() const override;

signals:
    void listReady();
    void fileOpened(const QString &filename);
    void failed();

private:
    QList<RemoteLayout> m_data;

    QNetworkAccessManager m_manager;
    TopicStore *m_store;
};

#endif // REMOTELAYOUTMODEL_H
