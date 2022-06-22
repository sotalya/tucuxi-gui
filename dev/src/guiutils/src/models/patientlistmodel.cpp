//@@license@@

#include "patientlistmodel.h"
#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"

#include <QDebug>

using namespace Tucuxi::Gui::Admin;

PatientListModel::PatientListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _patients(0),
    _roleNames()
{
    init();
}

PatientListModel::PatientListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _patients(0),
    _roleNames()
{
    init();
}

PatientListModel::~PatientListModel()
{

}

Qt::ItemFlags PatientListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> PatientListModel::roleNames() const
{
    return _roleNames;
}

QVariant PatientListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case ExternalIdRole:
        return static_cast<Patient*>(_patients->at(index.row()))->externalId();
        break;
    case StayNumberRole:
        return static_cast<Patient*>(_patients->at(index.row()))->stayNumber();
        break;
    case StatOkRole:
        return static_cast<Patient*>(_patients->at(index.row()))->statOk();
        break;
    case FirstNameRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->firstname();
        break;
    case LastNameRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->name();
        break;
    case BirthDateRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->birthday();
        break;
    case GenderRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->gender();
        break;
    case AddressRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->location()->address();
        break;
    case PostcodeRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->location()->postcode();
        break;
    case CityRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->location()->city();
        break;
    case StateRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->location()->state();
        break;
    case CountryRole:
        return static_cast<Patient*>(_patients->at(index.row()))->person()->location()->country();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int PatientListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (_patients) {
        return _patients->count();
    } else {
        return 0;
    }
}

void PatientListModel::setModelData(QList<SharedPatient> *patients)
{
    beginResetModel();
    _patients = patients;
    endResetModel();
    emit updateStatus();
}

void PatientListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case ExternalIdRole:
        static_cast<Patient*>(_patients->at(row))->externalId(value.toString());
        return;
    case StayNumberRole:
        static_cast<Patient*>(_patients->at(row))->stayNumber(value.toString());
        return;
    case StatOkRole:
        static_cast<Patient*>(_patients->at(row))->statOk(value.toBool());
        return;
    case FirstNameRole:
        static_cast<Patient*>(_patients->at(row))->person()->firstname(value.toString());
        return;
    case LastNameRole:
        static_cast<Patient*>(_patients->at(row))->person()->name(value.toString());
        return;
    case BirthDateRole:
        static_cast<Patient*>(_patients->at(row))->person()->birthday(value.toDate());
        return;
    case GenderRole:
        static_cast<Patient*>(_patients->at(row))->person()->gender(Person::GenderType(value.toInt()));
        return;
    case AddressRole:
        static_cast<Patient*>(_patients->at(row))->person()->location()->address(value.toString());
        return;
    case PostcodeRole:
        static_cast<Patient*>(_patients->at(row))->person()->location()->postcode(value.toString());
        return;
    case CityRole:
        static_cast<Patient*>(_patients->at(row))->person()->location()->city(value.toString());
        return;
    case StateRole:
        static_cast<Patient*>(_patients->at(row))->person()->location()->state(value.toString());
        return;
    case CountryRole:
        static_cast<Patient*>(_patients->at(row))->person()->location()->country(value.toString());
        return;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void PatientListModel::insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);

    SharedPatient patient;
    for(int i = at, j = 0; i < at + count; i++, j++) {

        //Setup or retrieve the patient
        if (entities.isEmpty()) {
            patient = AdminFactory::createEntity<Patient>(ABSTRACTREPO);
            static_cast<Patient*>(patient)->person()->birthday(QDate(1970, 1, 1));
            static_cast<Patient*>(patient)->person()->gender(Person::Male);
        } else {
            patient = static_cast<Patient*>(entities.at(j));
        }

        _patients->insert(i, patient);
    }
    emit updateStatus();
}

Tucuxi::Gui::Core::SharedEntitySet PatientListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    Tucuxi::Gui::Core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _patients->takeAt(at);

    emit updateStatus();
    return removed;
}

void PatientListModel::init()
{
    _roleNames[ExternalIdRole] = "externalId";
    _roleNames[StayNumberRole] = "stayNumber";
    _roleNames[StatOkRole]     = "statOk";
    _roleNames[FirstNameRole]  = "firstName";
    _roleNames[LastNameRole]   = "lastName";
    _roleNames[BirthDateRole]  = "birthDate";
    _roleNames[GenderRole]     = "gender";
    _roleNames[AddressRole]    = "address";
    _roleNames[PostcodeRole]   = "postcode";
    _roleNames[CityRole]       = "city";
    _roleNames[StateRole]      = "state";
    _roleNames[CountryRole]    = "country";
}
