//@@license@@

#include "infusionlistmodel.h"
#include "core/dal/drug/infusions.h"

InfusionListModel::InfusionListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _infusions(),
    _roleNames()
{
    init();
}

InfusionListModel::InfusionListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _infusions(),
    _roleNames()
{
    init();
}

InfusionListModel::~InfusionListModel()
{

}

Qt::ItemFlags InfusionListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> InfusionListModel::roleNames() const
{
    return _roleNames;
}

QVariant InfusionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

int i = index.row();//TOERASE

    switch (role) {

    case InfusionDefaultRole:
        return _infusions->getQuantity()->value();
    case InfusionUnitRole:
        return _infusions->getQuantity()->getUnitstring();
    case InfusionAnyRole:
        return _infusions->getAny();
    case InfusionRole:
        return _infusions->at(index.row())->getQuantity()->value();
    default:
        Q_ASSERT(false);
    }

    return QVariant();
}

int InfusionListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(_infusions)
        return _infusions->size();
    else
        return 0;
}

void InfusionListModel::setModelData( ezechiel::core::ValidInfusions *infusions)
{
    beginResetModel();
    _infusions = infusions;
    endResetModel();

}

void InfusionListModel::setModelData(ezechiel::core::DrugModel *drug)
{
    Q_ASSERT(drug);

    beginResetModel();
    _infusions = drug->getInfusions();
    endResetModel();
}

//void DoseListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role){    Q_ASSERT(index.isValid());    Q_ASSERT(value.isValid());    Q_ASSERT(_roleNames.contains(role));    int row = index.row();    Q_ASSERT(row < rowCount());}

//void DoseListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities){}

//ezechiel::core::SharedEntitySet DoseListModel::removeRowsImpl(int at, int count){Q_ASSERT(at >= 0 && at + count <= rowCount());Q_ASSERT(count > 0);}

void InfusionListModel::init()
{

    _roleNames[InfusionDefaultRole] = "infusionDefault";
    _roleNames[InfusionUnitRole] = "infusionUnit";
    _roleNames[InfusionAnyRole] = "infusionAny";
    _roleNames[InfusionRole] = "infusion";

}
