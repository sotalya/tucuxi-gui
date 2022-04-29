//@@license@@

#include <QtGlobal>
//#include <QBrush>

#include "partialrequestlistmodel.h"

#include "admin/src/stdadminrepository.h"
#include "admin/src/dal/patient.h"
#include "admin/src/dal/person.h"
#include "admin/src/dal/measure.h"
#include "admin/src/adminfactory.h"

#include "apputils/src/appcore.h"
#include "apputils/src/drugmanager.h"

#include "core/dal/drug/translatablestring.h"
#include "core/dal/drug/drug.h"
#include "core/utils/ezutils.h"


PartialRequestListModel::PartialRequestListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _requests(),
    _roleNames()
{
    init();
}

PartialRequestListModel::PartialRequestListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _requests(),
    _roleNames()
{
    init();
}

PartialRequestListModel::~PartialRequestListModel()
{

}

QHash<int, QByteArray> PartialRequestListModel::roleNames() const
{
    return _roleNames;
}

QVariant PartialRequestListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case RequestIdRole:
        return _requests.at(index.row())->requestId();
        break;
    case RequestCpclRole:
        return _requests.at(index.row())->requestCpcl();
        break;
    case PatientIdRole:
        return static_cast<Patient*>(_requests.at(index.row())->patient())->externalId();
        break;
    case FirstNameRole:
        return static_cast<Patient*>(_requests.at(index.row())->patient())->person()->firstname();
        break;
    case LastNameRole:
        return static_cast<Patient*>(_requests.at(index.row())->patient())->person()->name();
        break;
    case InstituteRole:
        return _requests.at(index.row())->institute()->name();
        break;
    case ValueRole:
        return _requests.at(index.row())->sample()->getConcentration()->value();
        break;
    case UnitRole:
        return _requests.at(index.row())->sample()->getConcentration()->unit().name();
        break;
    case DateRole:
        return _requests.at(index.row())->sample()->getMoment();
        break;
    case DrugRole:
//        return _requests.at(index.row())->drug()->getName()->value();
        if (_requests.at(index.row())->uncastedSourceSubstance().isEmpty())
            return _requests.at(index.row())->drug()->getName()->value();
        else return QString("Unknown: ") + _requests.at(index.row())->uncastedSourceSubstance();
        break;
    case DrugIdRole:
        return _requests.at(index.row())->drug()->getSubstanceId();
        break;
    case ColorRole:
        return data(index, AlarmRole).toBool() ? "darkred" : "black";
        break;
    case AlarmRole:
        return false; //return _requests.at(index.row())->sample()->value() > 3000; //ToDo: replace by threshold from drug file
        break;
    }

//    Q_ASSERT(false);
    return QVariant();
}

int PartialRequestListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _requests.count();
}

bool PartialRequestListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return EXIT_FAILURE;
}

bool PartialRequestListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return EXIT_FAILURE;
}

bool PartialRequestListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return EXIT_FAILURE;
}

void PartialRequestListModel::setModelData(const QList<SharedPartialRequest> &requests)
{
    beginResetModel();
    _requests = requests;
    endResetModel();
}

void PartialRequestListModel::init()
{
    _roleNames[RequestIdRole]   = "requestId";
    _roleNames[RequestCpclRole] = "requestCpcl";
    _roleNames[PatientIdRole]   = "patientId";
    _roleNames[FirstNameRole]   = "patientFirstName";
    _roleNames[LastNameRole]    = "patientLastName";
    _roleNames[InstituteRole]   = "institute";
    _roleNames[ValueRole]       = "sampleValue";
    _roleNames[UnitRole]        = "sampleUnit";
    _roleNames[DateRole]        = "sampleDate";
    _roleNames[DrugRole]        = "drug";
    _roleNames[DrugIdRole]      = "drugId";
    _roleNames[ColorRole]       = "color";
    _roleNames[AlarmRole]       = "alarm";
}
