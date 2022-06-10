//@@license@@

#include "adjustmentlistmodel.h"
#include "core/utils/errorenums.h"

AdjustmentListModel::AdjustmentListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _roleNames(),
    _pred(0)
{
    init();
}

AdjustmentListModel::AdjustmentListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _roleNames(),
    _pred(0)
{
    init();
}

AdjustmentListModel::~AdjustmentListModel()
{

}

bool AdjustmentListModel::clearModel()
{
    beginResetModel();
    _pred = 0;
    endResetModel();
    emit updateStatus();
    return true;
}

QHash<int, QByteArray> AdjustmentListModel::roleNames() const
{
    return _roleNames;
}

QVariant AdjustmentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case ValueRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getQuantity()->value();
        break;
    case UnitRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getQuantity()->unit().name();
        break;
    case AppliedRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getApplied();
        break;
    case EndtimeRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getEndTime();
        break;
    case TinfRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getTinf().toMinutes();
        break;
    case IntervalRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getDbinterval();
        break;
    case RouteRole:
        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->first()->getRoute()->getValue();
        break;
    case TakenRole:
//        return _pred->getAdjustments()->at(index.row())->getDosageHistory()->taken();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int AdjustmentListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!_pred || !_pred->getAdjustments() || _pred->getAdjustments()->isEmpty()) {
        return 0;
    } else {
        return _pred->getAdjustments()->size();
    }
}

bool AdjustmentListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return false;
}

bool AdjustmentListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

bool AdjustmentListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

void AdjustmentListModel::setModelData(Tucuxi::Gui::Core::PredictionResult* pred)
{
    beginResetModel();
    _pred = pred;
    endResetModel();
    emit updateStatus();
}

void AdjustmentListModel::init()
{
    _roleNames[ValueRole]    = "value";
    _roleNames[IntervalRole] = "interval";
    _roleNames[TinfRole]     = "tinf";
    _roleNames[EndtimeRole]  = "endtime";
    _roleNames[AppliedRole]  = "applied";
    _roleNames[UnitRole]     = "unit";
    _roleNames[RouteRole]    = "route";
    _roleNames[TakenRole]	 = "taken";
}
