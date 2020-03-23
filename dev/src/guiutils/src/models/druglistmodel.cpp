#include "druglistmodel.h"
#include "core/corefactory.h"
#include "apputils/src/apputilsrepository.h"
#include "core/utils/logging.h"
#include "errors_guiutils.h"
#include "core/dal/drug/translatablestring.h"

DrugListModel::DrugListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _drugs(0),
    _roleNames()
{
    init();
}

DrugListModel::DrugListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _drugs(0),
    _roleNames()
{
    init();
}

DrugListModel::~DrugListModel()
{

}

QHash<int, QByteArray> DrugListModel::roleNames() const
{
    return _roleNames;
}

QVariant DrugListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return (_drugs->at(index.row())->getName()->value());
        break;
    case DescriptionRole:
        return _drugs->at(index.row())->getName()->value();
        break;
    case StudynameRole:
        return _drugs->at(index.row())->getStudyName();
        break;
    case DomainnameRole:
        return _drugs->at(index.row())->getDomainName();
        break;
    case AtcRole:
        return _drugs->at(index.row())->getActiveSubstance()->getAtc();
        break;
    case IndicationRole:
        return _drugs->at(index.row())->getIndication();
        break;
    case PopulationRole:
        return _drugs->at(index.row())->getPopulation();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int DrugListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (_drugs) {
        return _drugs->count();
    } else {
        return 0;
    }
}

bool DrugListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return false;
}

bool DrugListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

bool DrugListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

void DrugListModel::setModelData(QList<ezechiel::core::DrugModel*> *drugs)
{
    beginResetModel();
    _drugs = drugs;
    endResetModel();
    emit updateStatus();
}

void DrugListModel::init()
{
    _roleNames[NameRole]        = "name";
    _roleNames[DescriptionRole] = "description";
    _roleNames[StudynameRole]   = "studyname";
    _roleNames[DomainnameRole]  = "domainname";
    _roleNames[AtcRole]         = "atc";
    _roleNames[IndicationRole]  = "indication";
    _roleNames[PopulationRole]  = "population";
}
