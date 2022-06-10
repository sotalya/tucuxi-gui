//@@license@@

#include "institutelistmodel.h"
#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"

InstituteListModel::InstituteListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _institutes(0),
    _roleNames()
{
    init();
}

InstituteListModel::InstituteListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _institutes(0),
    _roleNames()
{
    init();
}

InstituteListModel::~InstituteListModel()
{

}

Qt::ItemFlags InstituteListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> InstituteListModel::roleNames() const
{
    return _roleNames;
}

QVariant InstituteListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return _institutes->at(index.row())->name();
        break;
    case ExternalIdRole:
        return _institutes->at(index.row())->externalId();
        break;
    case LogoRole:
        return _institutes->at(index.row())->logo();
        break;
    case AddressRole:
        return _institutes->at(index.row())->location()->address();
        break;
    case PostcodeRole:
        return _institutes->at(index.row())->location()->postcode();
        break;
    case CityRole:
        return _institutes->at(index.row())->location()->city();
        break;
    case StateRole:
        return _institutes->at(index.row())->location()->state();
        break;
    case CountryRole:
        return _institutes->at(index.row())->location()->country();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int InstituteListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _institutes->count();
}

void InstituteListModel::setModelData(QList<SharedInstitute> *institutes)
{
    beginResetModel();
    _institutes = institutes;
    endResetModel();
    emit updateStatus();
}

void InstituteListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case NameRole:
        _institutes->at(row)->name(value.toString());
        return;
    case ExternalIdRole:
        _institutes->at(row)->externalId(value.toString());
        return;
    case LogoRole:
        _institutes->at(row)->logo(value.value<QImage>());
        return;
    case AddressRole:
        _institutes->at(row)->location()->address(value.toString());
        return;
    case PostcodeRole:
        _institutes->at(row)->location()->postcode(value.toString());
        return;
    case CityRole:
        _institutes->at(row)->location()->city(value.toString());
        return;
    case StateRole:
        _institutes->at(row)->location()->state(value.toString());
        return;
    case CountryRole:
        _institutes->at(row)->location()->country(value.toString());
        return;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void InstituteListModel::insertRowsImpl(int at, int count, const Tucuxi::GuiCore::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);

    SharedInstitute institute;
    for(int i = at, j = 0; i < at + count; i++, j++) {
        institute = entities.isEmpty() ?
                    AdminFactory::createEntity<Institute>(ABSTRACTREPO) :
                    static_cast<Institute*>(entities.at(j));
        _institutes->insert(i, institute);
    }
    emit updateStatus();
}

Tucuxi::GuiCore::SharedEntitySet InstituteListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    Tucuxi::GuiCore::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _institutes->takeAt(at);

    return removed;
}

void InstituteListModel::init()
{
    _roleNames[NameRole]       = "name";
    _roleNames[ExternalIdRole] = "externalId";
    _roleNames[LogoRole]       = "logo";
    _roleNames[AddressRole]    = "address";
    _roleNames[PostcodeRole]   = "postcode";
    _roleNames[CityRole]       = "city";
    _roleNames[StateRole]      = "state";
    _roleNames[CountryRole]    = "country";
}
