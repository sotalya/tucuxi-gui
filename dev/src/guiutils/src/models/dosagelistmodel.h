//@@license@@

#ifndef DOSAGELISTMODEL_H
#define DOSAGELISTMODEL_H

#include "abstractentitylistmodel.h"

#include "core/dal/dosage.h"
#include "core/dal/drug/drug.h"

class DosageListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum DosageRoles {
        ValueRole,
        AppliedRole,
        EndtimeRole,
        IntervalRole,
        TinfRole,
        RouteRole,
        UnitRole,
        TakenRole
    };
    Q_ENUM(DosageRoles)

    explicit DosageListModel(QObject *parent = nullptr);
    explicit DosageListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~DosageListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(Tucuxi::GuiCore::DosageHistory* dosages);
    void setModelData(Tucuxi::GuiCore::DrugModel* drug);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual void insertRowsImpl(int at, int count, const Tucuxi::GuiCore::SharedEntitySet &entities = Tucuxi::GuiCore::SharedEntitySet());
    virtual Tucuxi::GuiCore::SharedEntitySet removeRowsImpl(int at, int count);

    void init();

    Tucuxi::GuiCore::DosageHistory* _dosages;
    Tucuxi::GuiCore::DrugModel* _drug;
    QHash<int, QByteArray> _roleNames;
};

#endif // DOSAGELISTMODEL_H
