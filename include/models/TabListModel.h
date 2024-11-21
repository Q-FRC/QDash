#ifndef TABLISTMODEL_H
#define TABLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <qdir.h>
#include <qqmlintegration.h>

#include "SettingsManager.h"
#include "TabWidgetsModel.h"

typedef struct {
    QString title;

    int rows;
    int cols;

    TabWidgetsModel *model;
} Tab;

class TabListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int selectedTab READ selectedTab NOTIFY selectedTabChanged FINAL)
public:
    enum TLMRoleTypes {
        TITLE = Qt::UserRole,
        ROWS,
        COLS,
        WIDGETS
    };

    explicit TabListModel(SettingsManager *settings = nullptr, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    Q_INVOKABLE void add(Tab t);
    Q_INVOKABLE void add(QString title);

    // Remove data:
    Q_INVOKABLE bool remove(int row, const QModelIndex &parent = QModelIndex());

    Q_INVOKABLE void save(const QString &filename = "");
    Q_INVOKABLE QJsonDocument saveObject() const;
    Q_INVOKABLE void loadObject(const QJsonDocument &doc);
    Q_INVOKABLE void load(const QString &fileName = "");

    int selectedTab() const;
    void selectTab(const QString &tab);

signals:
    void selectedTabChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Tab> m_data;

    SettingsManager *m_settings;
    int m_selectedTab = 0;
};
#endif // TABLISTMODEL_H
