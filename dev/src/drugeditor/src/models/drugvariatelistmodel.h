//@@license@@

#ifndef DRUGVARIATELISTMODEL_H
#define DRUGVARIATELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/drugvariate.h"

class DrugVariateListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DrugVariateRoles {
        NameRole,
        DescriptionRole,
        ValueRole,
        UnitRole,
        TypeRole,
        DateRole
    };
    Q_ENUM(DrugVariateRoles)

    explicit DrugVariateListModel(QObject *parent = 0);
    explicit DrugVariateListModel(QUndoStack *undoStack, QObject *parent = 0);
    virtual ~DrugVariateListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(ezechiel::GuiCore::DrugVariateList *drugVariates);

private:
    void init();

    ezechiel::GuiCore::DrugVariateList * _drugVariates;
    QHash<int, QByteArray> _roleNames;
};

#endif // DRUGVARIATELISTMODEL_H
