#include "intervallistmodel.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"

IntervalListModel::IntervalListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _intervals(),
    _roleNames()
{
    init();
}

IntervalListModel::IntervalListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _intervals(),
    _roleNames()
{
    init();
}

IntervalListModel::~IntervalListModel()
{

}

Qt::ItemFlags IntervalListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> IntervalListModel::roleNames() const
{
    return _roleNames;
}

QVariant IntervalListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

int i = index.row();//TOERASE

    switch (role) {

    case IntervalDefaultRole:
        return _intervals->getQuantity()->value();
    case IntervalUnitRole:
        return _intervals->getQuantity()->unit().name();
    case IntervalAnyRole:
        return _intervals->getAny();
    case IntervalRole:
        return _intervals->at(index.row())->getQuantity()->value();
    default:
        Q_ASSERT(false);
    }

    return QVariant();
}

int IntervalListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(_intervals)
        return _intervals->count();
    else
        return 0;
}

void IntervalListModel::setModelData(ezechiel::core::ValidIntervals *intervals)
{
    beginResetModel();
    _intervals = intervals;
    endResetModel();

}

void IntervalListModel::setModelData(ezechiel::core::DrugModel *drug)
{
    Q_ASSERT(drug);

    beginResetModel();
    _intervals = drug->getIntervals();
    endResetModel();
}

//void DoseListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role){    Q_ASSERT(index.isValid());    Q_ASSERT(value.isValid());    Q_ASSERT(_roleNames.contains(role));    int row = index.row();    Q_ASSERT(row < rowCount());}

//void DoseListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities){}

//ezechiel::core::SharedEntitySet DoseListModel::removeRowsImpl(int at, int count){Q_ASSERT(at >= 0 && at + count <= rowCount());Q_ASSERT(count > 0);}

void IntervalListModel::init()
{

    _roleNames[IntervalDefaultRole] = "intervalDefault";
    _roleNames[IntervalUnitRole] = "intervalUnit";
    _roleNames[IntervalAnyRole] = "intervalAny";
    _roleNames[IntervalRole] = "interval";

}
