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

//#define TESTING_TABLE

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

static PartialRequestListModel* sCurrentModel;

PartialRequestListModel::PartialRequestListModel(QObject *parent) :
    QAbstractTableModel(parent),
    _requests(),
    _roleNames()
{
    init();
    sCurrentModel = this;
}

PartialRequestListModel* PartialRequestListModel::currentModel()
{
    return sCurrentModel;
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
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(0).arg(index.row());
#endif
        return _requests.at(index.row())->requestId();
        break;
    case RequestCpclRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(1).arg(index.row());
#endif
        return _requests.at(index.row())->requestCpcl();
        break;
    case PatientIdRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(2).arg(index.row());
#endif
        return static_cast<Patient*>(_requests.at(index.row())->patient())->externalId();
        break;
    case FirstNameRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(3).arg(index.row());
#endif
        return static_cast<Patient*>(_requests.at(index.row())->patient())->person()->firstname();
        break;
    case LastNameRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(4).arg(index.row());
#endif
        return static_cast<Patient*>(_requests.at(index.row())->patient())->person()->name();
        break;
    case InstituteRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(5).arg(index.row());
#endif
        return _requests.at(index.row())->institute()->name();
        break;
    case ValueRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(6).arg(index.row());
#endif
        return _requests.at(index.row())->sample()->getConcentration()->value();
        break;
    case UnitRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(7).arg(index.row());
#endif
        return _requests.at(index.row())->sample()->getConcentration()->unit().name();
        break;
    case DateRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(8).arg(index.row());
#endif
        return _requests.at(index.row())->sample()->getMoment();
        break;
    case DrugRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(9).arg(index.row());
#endif
//        return _requests.at(index.row())->drug()->getName()->value();
        if (_requests.at(index.row())->uncastedSourceSubstance().isEmpty())
            return _requests.at(index.row())->drug()->getName()->value();
        else return QString("Unknown: ") + _requests.at(index.row())->uncastedSourceSubstance();
        break;
    case DrugIdRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(10).arg(index.row());
#endif
        return _requests.at(index.row())->drug()->getSubstanceId();
        break;
    case ColorRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(11).arg(index.row());
#endif
        return data(index, AlarmRole).toBool() ? "darkred" : "black";
        break;
    case AlarmRole:
#ifdef TESTING_TABLE
        return QString("%1, %2").arg(12).arg(index.row());
#endif
        return false; //return _requests.at(index.row())->sample()->value() > 3000; //ToDo: replace by threshold from drug file
        break;
    default:
        return QString("Testing");
        break;
    }

//    Q_ASSERT(false);
    return QVariant();
}

int PartialRequestListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
#ifdef TESTING_TABLE
    return 10;
#endif
    return _requests.count();

}

int PartialRequestListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant PartialRequestListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("ID");
        case 1:
            return QString("Patient");
        case 2:
            return QString("Institute");
        case 3:
            return QString("Drug");
        case 4:
            return QString("Last sample value");
        case 5:
            return QString("Sample date");
        case 6:
            return QString("Status");
        }
    }
    return QVariant();
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

bool PartialRequestListModel::clearModel()
{
    if (rowCount() > 0)
        removeRows(0, rowCount());

    return true;
}

QModelIndex PartialRequestListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
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
