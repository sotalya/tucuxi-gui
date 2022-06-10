//@@license@@

#include "doselistmodel.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/translatablestring.h"

DoseListModel::DoseListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _doses(),
    _roleNames()
{
    init();
}

DoseListModel::DoseListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _doses(),
    _roleNames()
{
    init();
}

DoseListModel::~DoseListModel()
{

}

Qt::ItemFlags DoseListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> DoseListModel::roleNames() const
{
    return _roleNames;
}

QVariant DoseListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {

    case DoseDefaultRole:
        return _doses->getQuantity()->value();
    case DoseUnitRole:
        return _doses->getQuantity()->unit().name();
    case DoseAnyRole:
        return _doses->getAnyDose();
    case DoseRole:
        return _doses->at(index.row())->getQuantity()->value();
    case CommentsRole:
        return (QVariant::fromValue(_doses->getComments()));
    default:
        Q_ASSERT(false);
    }

    return QVariant();
}

int DoseListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(_doses)
        return _doses->count();
    else
        return 0;
}

void DoseListModel::setModelData(Tucuxi::Gui::Core::ValidDoses* doses)
{
    beginResetModel();
    _doses = doses;
    endResetModel();

}

void DoseListModel::setModelData(Tucuxi::Gui::Core::DrugModel *drug)
{
    Q_ASSERT(drug);

    beginResetModel();
    _doses = drug->getDoses();
    endResetModel();
}

//void DoseListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role){    Q_ASSERT(index.isValid());    Q_ASSERT(value.isValid());    Q_ASSERT(_roleNames.contains(role));    int row = index.row();    Q_ASSERT(row < rowCount());}

//void DoseListModel::insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities){}

//Tucuxi::Gui::Core::SharedEntitySet DoseListModel::removeRowsImpl(int at, int count){Q_ASSERT(at >= 0 && at + count <= rowCount());Q_ASSERT(count > 0);}

void DoseListModel::init()
{

    _roleNames[DoseDefaultRole] = "doseDefault";
    _roleNames[DoseUnitRole] = "doseUnit";
    _roleNames[DoseAnyRole] = "doseAny";
    _roleNames[DoseRole] = "dose";
    _roleNames[CommentsRole] = "comments";

}
