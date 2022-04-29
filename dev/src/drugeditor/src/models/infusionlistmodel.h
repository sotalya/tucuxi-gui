//@@license@@

#ifndef DOSAGELISTMODEL_H
#define DOSAGELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/drug.h"

class InfusionListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DoseRoles {
        InfusionDefaultRole,
        InfusionUnitRole,
        InfusionAnyRole,
        InfusionRole,
    };
    Q_ENUM(DoseRoles)

    explicit InfusionListModel(QObject *parent = nullptr);
    explicit InfusionListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~InfusionListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

public slots:
    void setModelData(ezechiel::core::ValidInfusions *infusions);
    void setModelData(ezechiel::core::DrugModel* drug);

private:
//    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
//    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
//    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    ezechiel::core::ValidInfusions *_infusions;

    QHash<int, QByteArray> _roleNames;
};

#endif // DOSAGELISTMODEL_H
