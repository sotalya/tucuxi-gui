#ifndef DOMAINLISTMODEL_H
#define DOMAINLISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/utils/ezutils.h"

class DomainListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DomainRoles {
        IdRole,
        NameRole,
        DescriptionRole
    };
    Q_ENUM(DomainRoles)

    explicit DomainListModel(QObject *parent = 0);
    explicit DomainListModel(QUndoStack *undoStack, QObject *parent = 0);
    virtual ~DomainListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(const QList<ezechiel::core::Descriptor> &domains);

private:
    void init();

    QList<ezechiel::core::Descriptor> _domains;
    QHash<int, QByteArray> _roleNames;
};

#endif // DOMAINLISTMODEL_H
