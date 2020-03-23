#ifndef SORTFILTERENTITYLISTPROXYMODEL_H
#define SORTFILTERENTITYLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

class ProxyModelFilter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(Operation operation READ operation WRITE setOperation NOTIFY operationChanged)

    friend class SortFilterEntityListProxyModel;

public:
    enum Operation {
        Equal,
        Unequal,
        Less,
        LessOrEqual,
        Greater,
        GreaterOrEqual
    };
    Q_ENUM(Operation)

    Q_INVOKABLE explicit ProxyModelFilter(QObject *parent = 0);
    Q_INVOKABLE ProxyModelFilter(int role, QVariant value, Operation operation = Equal, QObject *parent = 0);
    Q_INVOKABLE virtual ~ProxyModelFilter() Q_DECL_OVERRIDE;

    Q_INVOKABLE int role() const;
    Q_INVOKABLE void setRole(int role);

    Q_INVOKABLE QVariant value() const;
    Q_INVOKABLE void setValue(const QVariant &value);

    Q_INVOKABLE Operation operation() const;
    Q_INVOKABLE void setOperation(Operation operation);

public slots:
    void setFixedString(const QString &pattern, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    void setRegExp(const QString &pattern, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    void setWildcard(const QString &pattern, Qt::CaseSensitivity cs = Qt::CaseSensitive);

signals:
    void roleChanged(int role);
    void valueChanged(const QVariant &value);
    void operationChanged(Operation operation);

private:
    bool accept(const QModelIndex &index) const;

    template<typename T>
    bool compare(const T &data, const T &value) const
    {
        switch (operation()) {
        case Equal:
            return data == value;
        case Unequal:
            return data != value;
        case Less:
            return data < value;
        case LessOrEqual:
            return data <= value;
        case Greater:
            return data > value;
        case GreaterOrEqual:
            return data >= value;
        default:
            Q_ASSERT(false);
        }
    }

    int _role;
    QVariant _value;
    Operation _operation;
};

inline bool operator== (const ProxyModelFilter &lhs, const ProxyModelFilter &rhs) { return lhs.role() == rhs.role() && lhs.value() == rhs.value() && lhs.operation() == rhs.operation(); }
inline bool operator!= (const ProxyModelFilter &lhs, const ProxyModelFilter &rhs) { return !(lhs == rhs); }

class SortFilterEntityListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SortFilterEntityListProxyModel(QObject *parent = 0);
    virtual ~SortFilterEntityListProxyModel();

    virtual void setSourceModel(QAbstractItemModel *sourceModel) Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual void sort(int role, Qt::SortOrder order) Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool appendRows(int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE virtual bool prependRows(int count, const QModelIndex &parent = QModelIndex());

    Q_INVOKABLE virtual void removeFilter(int role);
    Q_INVOKABLE virtual void setFilter(ProxyModelFilter *filter);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

protected slots:
    virtual void filterChanged();

private:
    QMultiHash<int, ProxyModelFilter *> _filters;
};

#endif // SORTFILTERENTITYLISTPROXYMODEL_H
