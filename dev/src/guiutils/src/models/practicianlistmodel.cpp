#include "practicianlistmodel.h"
#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"

PracticianListModel::PracticianListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _practicians(0),
    _roleNames()
{
    init();
}

PracticianListModel::PracticianListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _practicians(0),
    _roleNames()
{
    init();
}

PracticianListModel::~PracticianListModel()
{

}

Qt::ItemFlags PracticianListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> PracticianListModel::roleNames() const
{
    return _roleNames;
}

QVariant PracticianListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case ExternalIdRole:
        return _practicians->at(index.row())->externalId();
        break;
    case TitleRole:
        return _practicians->at(index.row())->title();
        break;
    case InstituteIdRole:
        return _practicians->at(index.row())->institute_id();
        break;
    case FirstNameRole:
        return _practicians->at(index.row())->person()->firstname();
        break;
    case LastNameRole:
        return _practicians->at(index.row())->person()->name();
        break;
    case BirthDateRole:
        return _practicians->at(index.row())->person()->birthday();
        break;
    case GenderRole:
        return _practicians->at(index.row())->person()->gender();
        break;
    case AddressRole:
        return _practicians->at(index.row())->person()->location()->address();
        break;
    case PostcodeRole:
        return _practicians->at(index.row())->person()->location()->postcode();
        break;
    case CityRole:
        return _practicians->at(index.row())->person()->location()->city();
        break;
    case StateRole:
        return _practicians->at(index.row())->person()->location()->state();
        break;
    case CountryRole:
        return _practicians->at(index.row())->person()->location()->country();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int PracticianListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _practicians->count();
}

void PracticianListModel::setModelData(QList<SharedPractician> *practicians)
{
    beginResetModel();
    _practicians = practicians;
    endResetModel();
    emit updateStatus();
}

void PracticianListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case ExternalIdRole:
        _practicians->at(row)->externalId(value.toString());
        return;
    case TitleRole:
        _practicians->at(row)->title(value.toString());
        return;
    case InstituteIdRole:
        _practicians->at(row)->institute_id(value.toInt());
        return;
    case FirstNameRole:
        _practicians->at(row)->person()->firstname(value.toString());
        return;
    case LastNameRole:
        _practicians->at(row)->person()->name(value.toString());
        return;
    case BirthDateRole:
        _practicians->at(row)->person()->birthday(value.toDate());
        return;
    case GenderRole:
        _practicians->at(row)->person()->gender(Person::GenderType(value.toInt()));
        return;
    case AddressRole:
        _practicians->at(row)->person()->location()->address(value.toString());
        return;
    case PostcodeRole:
        _practicians->at(row)->person()->location()->postcode(value.toString());
        return;
    case CityRole:
        _practicians->at(row)->person()->location()->city(value.toString());
        return;
    case StateRole:
        _practicians->at(row)->person()->location()->state(value.toString());
        return;
    case CountryRole:
        _practicians->at(row)->person()->location()->country(value.toString());
        return;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void PracticianListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);

    SharedPractician practician;
    for(int i = at, j = 0; i < at + count; i++, j++) {
        practician = entities.isEmpty() ?
                     AdminFactory::createEntity<Practician>(ABSTRACTREPO) :
                     static_cast<Practician*>(entities.at(j));
        _practicians->insert(i, practician);
    }
    emit updateStatus();
}

ezechiel::core::SharedEntitySet PracticianListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    ezechiel::core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _practicians->takeAt(at);

    emit updateStatus();
    return removed;
}

void PracticianListModel::init()
{
    _roleNames[ExternalIdRole]  = "externalId";
    _roleNames[TitleRole]       = "title";
    _roleNames[InstituteIdRole] = "instituteId";
    _roleNames[FirstNameRole]   = "firstName";
    _roleNames[LastNameRole]    = "lastName";
    _roleNames[BirthDateRole]   = "birthDate";
    _roleNames[GenderRole]      = "gender";
    _roleNames[AddressRole]     = "address";
    _roleNames[PostcodeRole]    = "postcode";
    _roleNames[CityRole]        = "city";
    _roleNames[StateRole]       = "state";
    _roleNames[CountryRole]     = "country";
}
