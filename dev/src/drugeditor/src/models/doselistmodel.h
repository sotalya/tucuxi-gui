#ifndef DOSAGELISTMODEL_H
#define DOSAGELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/drug/doses.h"
#include "core/dal/drug/drug.h"

class DoseListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DoseRoles {
        DoseDefaultRole,
        DoseUnitRole,
        DoseAnyRole,
        DoseRole,
        CommentsRole
    };
    Q_ENUM(DoseRoles)

    explicit DoseListModel(QObject *parent = nullptr);
    explicit DoseListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~DoseListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

public slots:
    void setModelData(ezechiel::core::ValidDoses* doses);
    void setModelData(ezechiel::core::DrugModel* drug);

private:
//    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
//    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
//    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    ezechiel::core::ValidDoses *_doses;
    QHash<int, QByteArray> _roleNames;
};

#endif // DOSAGELISTMODEL_H
