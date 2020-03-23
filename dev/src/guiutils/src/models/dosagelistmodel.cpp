#include "dosagelistmodel.h"
#include "core/corefactory.h"
#include "core/corerepository.h"
#include "core/utils/errorenums.h"
#include "core/core.h"
#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/adme.h"
#include "core/dal/drug/infusions.h"

DosageListModel::DosageListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _dosages(),
    _drug(),
    _roleNames()
{
    init();
}

DosageListModel::DosageListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _dosages(),
    _drug(),
    _roleNames()
{
    init();
}

DosageListModel::~DosageListModel()
{

}

Qt::ItemFlags DosageListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> DosageListModel::roleNames() const
{
    return _roleNames;
}

QVariant DosageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case ValueRole:
        return _dosages->at(index.row())->getQuantity()->value();
        break;
    case UnitRole:
        return _dosages->at(index.row())->getQuantity()->unit().name();
        break;
    case AppliedRole:
        return _dosages->at(index.row())->getApplied();
        break;
    case EndtimeRole:
        return _dosages->at(index.row())->getEndTime();
        break;
    case TinfRole:
        return _dosages->at(index.row())->getDbtinf();
        break;
    case IntervalRole:
        return _dosages->at(index.row())->getDbinterval();
        break;
    case RouteRole:
        return _dosages->at(index.row())->getRoute()->getValue();
        break;
//    case TakenRole:
//        return _dosages->at(index.row())->taken();
//        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int DosageListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (_dosages) {
        return _dosages->size();
    } else {
        return 0;
    }
}

void DosageListModel::setModelData(ezechiel::core::DosageHistory* dosages)
{
    beginResetModel();
    _dosages = dosages;
    endResetModel();
    emit updateStatus();
}

void DosageListModel::setModelData(ezechiel::core::DrugModel* drug)
{
    _drug = drug;
    emit updateStatus();
}

void DosageListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case ValueRole:
        _dosages->at(row)->getQuantity()->setValue(value.toDouble());
        break;
    case EndtimeRole:
        _dosages->at(row)->setEndTime(value.toDateTime());
        break;
    case AppliedRole:
        _dosages->at(row)->setApplied(value.toDateTime());
        break;
    case TinfRole:
        _dosages->at(row)->setDbtinf(value.toDouble());
        break;
    case IntervalRole:
        _dosages->at(row)->setDbinterval(value.toDouble());
        break;
    case RouteRole:        
        switch (value.toInt()) {
        case 0:
            _dosages->at(row)->getRoute()->setRoute(ezechiel::core::Admin::BOLUS);
            break;
        case 1:
            _dosages->at(row)->getRoute()->setRoute(ezechiel::core::Admin::INFUSION);
            break;
        case 2:
            _dosages->at(row)->getRoute()->setRoute(ezechiel::core::Admin::EXTRA);
            break;
        default:
            _dosages->at(row)->getRoute()->setRoute(ezechiel::core::Admin::BOLUS);
            break;
        }
        break;
    case UnitRole:
        _dosages->at(row)->getQuantity()->setUnit(value.toString());
        break;
//    case TakenRole:
//        _dosages->at(row)->setTaken(value.toBool());
//        break;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void DosageListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);
    Q_ASSERT(_drug);
    Q_ASSERT(_drug->getAdme());
    Q_ASSERT(_drug->getDoses());
    Q_ASSERT(_drug->getIntervals());
    Q_ASSERT(_drug->getInfusions());

    ezechiel::core::Dosage* dosage;
    for(int i = at, j = 0; i < at + count; i++, j++) {

        //Setup or retrieve the dosage
        if (entities.isEmpty()) {
            dosage = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Dosage>(ABSTRACTREPO);
//            dosage->setTaken(true);
            dosage->getQuantity()->setUnit(_drug->getDoses()->getQuantity()->unit().name());
            dosage->getQuantity()->setValue(_drug->getDoses()->getQuantity()->value());
            dosage->setDbinterval(_drug->getIntervals()->getQuantity()->value());

            dosage->setRoute(_drug->getAdme()->getDefaultIntake());
            if (dosage->getRoute()->getRoute() == ezechiel::core::Admin::INFUSION){
                dosage->setDbtinf(_drug->getInfusions()->getQuantity()->getDbvalue());
            }
        } else {
            dosage = static_cast<ezechiel::core::Dosage*>(entities.at(j));
        }

        _dosages->getList().insert(i, dosage);
    }
    emit updateStatus();
}

ezechiel::core::SharedEntitySet DosageListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    ezechiel::core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _dosages->getList().takeAt(at);

    emit updateStatus();
    return removed;
}

void DosageListModel::init()
{
    _roleNames[ValueRole]     = "value";
    _roleNames[IntervalRole]  = "interval";
    _roleNames[TinfRole]      = "tinf";
    _roleNames[EndtimeRole]   = "endtime";
    _roleNames[AppliedRole]   = "applied";
    _roleNames[UnitRole]      = "unit";
    _roleNames[RouteRole]     = "route";
//    _roleNames[TakenRole]	  = "taken";
}
