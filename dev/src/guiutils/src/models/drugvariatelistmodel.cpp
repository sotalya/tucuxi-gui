//@@license@@

#include "drugvariatelistmodel.h"
#include "core/corefactory.h"
#include "core/corerepository.h"
#include "admin/src/stdadminrepository.h"
#include "core/utils/logging.h"
#include "errors_guiutils.h"
#include "core/dal/drug/translatablestring.h"

DrugVariateListModel::DrugVariateListModel(QObject *parent) :
    AbstractEntityListModel( parent),
    _drugVariates(0),
    _roleNames()
{
    init();
}

DrugVariateListModel::DrugVariateListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _drugVariates(0),
    _roleNames()
{
    init();
}

DrugVariateListModel::~DrugVariateListModel()
{

}

QHash<int, QByteArray> DrugVariateListModel::roleNames() const
{
    return _roleNames;
}

QVariant DrugVariateListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return _drugVariates->at(index.row())->getVisualNameTranslation()->value("en");
        break;
    case DescriptionRole:
        return _drugVariates->at(index.row())->getDescription();
        break;
    case ValueRole:
        return _drugVariates->at(index.row())->getQuantity()->value();
        break;
    case UnitRole:
        return _drugVariates->at(index.row())->getQuantity()->unit().name();
        break;
    case TypeRole:
        return QMetaType::typeName(_drugVariates->at(index.row())->getType());
        break;
    case DateRole:
        // YTA : Removed this. Should be in PatientVariate, not DrugVariate
//        return _drugVariates->at(index.row())->getDate();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int DrugVariateListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!_drugVariates || _drugVariates->isEmpty()) {
        return 0;
    }
    return _drugVariates->getList().count();
}

bool DrugVariateListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return false;
}

bool DrugVariateListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

bool DrugVariateListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

void DrugVariateListModel::setModelData(Tucuxi::GuiCore::DrugVariateList *drugVariates)
{
    beginResetModel();
    _drugVariates = drugVariates;
    endResetModel();
    emit updateStatus();
}

void DrugVariateListModel::init()
{
    _roleNames[NameRole]        = "name";
    _roleNames[DescriptionRole] = "description";
    _roleNames[ValueRole]       = "value";
    _roleNames[UnitRole]        = "unit";
    _roleNames[TypeRole]        = "type";
    _roleNames[DateRole]        = "date";
}
