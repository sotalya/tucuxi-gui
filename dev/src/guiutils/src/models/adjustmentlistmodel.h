//@@license@@

#ifndef ADJUSTMENTLISTMODEL_H
#define ADJUSTMENTLISTMODEL_H

#include "abstractentitylistmodel.h"
#include "core/dal/dosage.h"
#include "core/dal/predictionresult.h"

class AdjustmentListModel : public AbstractEntityListModel
{
    Q_OBJECT

public:
    enum AdjustmentRoles {
        ValueRole,
        AppliedRole,
        EndtimeRole,
        IntervalRole,
        TinfRole,
        RouteRole,
        UnitRole,
        TakenRole
    };
    Q_ENUM(AdjustmentRoles)

    explicit AdjustmentListModel(QObject *parent = nullptr);
    explicit AdjustmentListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~AdjustmentListModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool clearModel() Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

signals:
    void updateStatus();

public slots:
    void setModelData(ezechiel::core::PredictionResult*);
//    void setModelData(const QList<ezechiel::core::Dosage*> &adjustments);

private:
    void init();

    ezechiel::core::PredictionResult* _pred;
//    QList<ezechiel::core::Dosage*> _adjustments;
    QHash<int, QByteArray> _roleNames;
};

#endif // ADJUSTMENTLISTMODEL_H
