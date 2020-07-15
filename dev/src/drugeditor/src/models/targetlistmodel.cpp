#include "targetlistmodel.h"
#include "adminfactory.h"
#include "stdadminrepository.h"
#include "corerepository.h"
#include "logging.h"
#include "errors_guiutils.h"
#include "apputilsrepository.h"
#include "results.h"

using namespace ezechiel::core;

TargetListModel::TargetListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _targets(0),
    _drug(),
    _roleNames()
{
    init();
}

TargetListModel::TargetListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _targets(0),
    _drug(),
    _roleNames()
{
    init();
}

TargetListModel::~TargetListModel()
{

}

Qt::ItemFlags TargetListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> TargetListModel::roleNames() const
{
    return _roleNames;
}

QVariant TargetListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case TypeRole:
        return Target::name(_targets->at(index.row())->getType());
        break;
    case CMinValueRole:
        return _targets->at(index.row())->getCmin()->value();
        break;
    case CMinUnitRole:
        return _targets->at(index.row())->getCmin()->unit().name();
        break;
    case CMaxValueRole:
        return _targets->at(index.row())->getCmax()->value();
        break;
    case CMaxUnitRole:
        return _targets->at(index.row())->getCmax()->unit().name();
        break;
    case CBestValueRole:
        return _targets->at(index.row())->getCbest()->value();
        break;
    case CBestUnitRole:
        return _targets->at(index.row())->getCbest()->unit().name();
        break;
    case TMinValueRole:
        return _targets->at(index.row())->getTmin()->value();
        break;
    case TMinUnitRole:
        return _targets->at(index.row())->getTmin()->unit().name();
        break;
    case TMaxValueRole:
        return _targets->at(index.row())->getTmax()->value();
        break;
    case TMaxUnitRole:
        return _targets->at(index.row())->getTmax()->unit().name();
        break;
    case TBestValueRole:
        return _targets->at(index.row())->getTbest()->value();
        break;
    case TBestUnitRole:
        return _targets->at(index.row())->getTbest()->unit().name();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int TargetListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _targets->count();
}

void TargetListModel::setModelData(QList<ezechiel::core::Target*> *targets)
{
    beginResetModel();
    _targets = targets;
    endResetModel();
    emit updateStatus();
}

void TargetListModel::setDefaultUnit(QString unitname) {
    _defaultCUnit = ezechiel::core::Unit(unitname);
}

void TargetListModel::setModelData(ezechiel::core::Drug* drug)
{
    _drug = drug;
    emit updateStatus();
}

void TargetListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case TypeRole:
        switch (value.toInt()) {
        case 0:
            _targets->at(row)->setType(ezechiel::core::ResidualTarget);
            break;
        case 1:
            _targets->at(row)->setType(ezechiel::core::PeakTarget);
            break;
        case 2:
            _targets->at(row)->setType(ezechiel::core::MeanTarget);
            break;
        default:
            _targets->at(row)->setType(ezechiel::core::UnknownTarget);
            break;
        }
        break;
        return;
    case CMinValueRole:
        _targets->at(row)->getCmin()->setValue(value.toDouble());
        return;
    case CMinUnitRole:
        _targets->at(row)->getCmin()->convert(Unit(value.toString()));
        return;
    case CMaxValueRole:
        _targets->at(row)->getCmax()->setValue(value.toDouble());
        return;
    case CMaxUnitRole:
        _targets->at(row)->getCmax()->convert(Unit(value.toString()));
        return;
    case CBestValueRole:
        _targets->at(row)->getCbest()->setValue(value.toDouble());
        return;
    case CBestUnitRole:
        _targets->at(row)->getCbest()->convert(Unit(value.toString()));
        return;
    case TMinValueRole:
        _targets->at(row)->getTmin()->setValue(value.toDouble());
        return;
    case TMinUnitRole:
        _targets->at(row)->getTmin()->convert(Unit(value.toString()));
        return;
    case TMaxValueRole:
        _targets->at(row)->getTmax()->setValue(value.toDouble());
        return;
    case TMaxUnitRole:
        _targets->at(row)->getTmax()->convert(Unit(value.toString()));
        return;
    case TBestValueRole:
        _targets->at(row)->getTbest()->setValue(value.toDouble());
        return;
    case TBestUnitRole:
        _targets->at(row)->getTbest()->convert(Unit(value.toString()));
        return;
    }
    emit updateStatus();
}

void TargetListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);
    Q_ASSERT(_drug);
    Q_ASSERT(_drug->getResults());

    // ToDo-Other /////////////////////////////////////////////////////////////
    // The target dal object needs to be refactored and the targets default
    // unit for the concentrations and the time (peak targets) should be
    // made available from the drug object directly.
    // ////////////////////////////////////////////////////////////////////////

    Target* target;
    for(int i = at, j = 0; i < at + count; i++, j++) {

        //Setup or retrieve the dosage
        if (entities.isEmpty()) {
            target = CoreFactory::createEntity<Target>(ABSTRACTREPO);
            target->setType(ezechiel::core::ResidualTarget);
//            target->setCunit(!_drug->getTargets()->isEmpty() ? _drug->getTargets()->first()->getCunit() :_drug->getResults()->getUnit());
            target->getCmin()->setUnit(_defaultCUnit);
            target->getCmax()->setUnit(_defaultCUnit);
            target->getCbest()->setUnit(_defaultCUnit);
        } else {
            target = static_cast<Target*>(entities.at(j));
        }
        _targets->insert(i, target);
    }
    emit updateStatus();
}

ezechiel::core::SharedEntitySet TargetListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    ezechiel::core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _targets->takeAt(at);

    emit updateStatus();
    return removed;
}

void TargetListModel::init()
{
    _roleNames[TypeRole]       = "type";
    _roleNames[CMinValueRole]  = "cminvalue";
    _roleNames[CMinUnitRole]   = "cminunit";
    _roleNames[CMaxValueRole]  = "cmaxvalue";
    _roleNames[CMaxUnitRole]   = "cmaxunit";
    _roleNames[CBestValueRole] = "cbestvalue";
    _roleNames[CBestUnitRole]  = "cbestunit";
    _roleNames[TMinValueRole]  = "tminvalue";
    _roleNames[TMinUnitRole]   = "tminunit";
    _roleNames[TMaxValueRole]  = "tmaxvalue";
    _roleNames[TMaxUnitRole]   = "tmaxunit";
    _roleNames[TBestValueRole] = "tbestvalue";
    _roleNames[TBestUnitRole]  = "tbestunit";
}