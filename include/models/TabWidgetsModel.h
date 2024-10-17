#ifndef TABWIDGETSMODEL_H
#define TABWIDGETSMODEL_H

#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QRectF>

typedef struct Widget {
    Q_GADGET
    QML_ELEMENT

public:
    QString title;
    QString topic;

    QMetaType dataType;
    QString type;

    int row;
    int col;
    int rowSpan;
    int colSpan;

    QVariantMap properties;
} Widget;

class TabWidgetsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum TWMRoleTypes {
        TITLE = Qt::UserRole,
        ROW,
        COL,
        ROWSPAN,
        COLSPAN,
        TYPE,
        TOPIC,
        PROPERTIES,
        IDX
    };

    explicit TabWidgetsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    Q_INVOKABLE Widget copy(int idx);
    Q_INVOKABLE void add(Widget w);
    Q_INVOKABLE void add(QString title, QString topic, QString type);
    Q_INVOKABLE void addCamera(QString name, QString source, QVariantList urls);

    Q_INVOKABLE void setEqualTo(TabWidgetsModel *w);

    QList<Widget> data();

    // Remove data:
    Q_INVOKABLE bool remove(int idx);
    Q_INVOKABLE bool removeLatest();

    int rows() const;
    void setRows(int newRows);

    int cols() const;
    void setCols(int newCols);

    Q_INVOKABLE bool cellOccupied(int row, int col, int rowSpan = 1, int colSpan = 1, QRectF ignore = QRectF(-1, -1, -1, -1));

    int unoccupiedCells() const;
    void setUnoccupiedCells(int newUnoccupiedCells);

    QJsonArray saveObject() const;
    static TabWidgetsModel *loadObject(QObject *parent, const QJsonArray &arr);

protected:
    QHash<int, QByteArray> roleNames() const override;


signals:
    void rowsChanged();

    void colsChanged();

    void unoccupiedCellsChanged();

private:
    QList<Widget> m_data;

    int m_rows;
    int m_cols;

    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged FINAL)
    Q_PROPERTY(int cols READ cols WRITE setCols NOTIFY colsChanged FINAL)
    Q_PROPERTY(int unoccupiedCells READ unoccupiedCells NOTIFY unoccupiedCellsChanged FINAL)
};

Q_DECLARE_METATYPE(TabWidgetsModel)

#endif // TABWIDGETSMODEL_H
