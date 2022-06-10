//@@license@@

#include "targetmodel.h"
#include "errors_guiutils.h"

#include "core/corerepository.h"
#include "core/utils/logging.h"
#include "core/dal/drug/drug.h"

#include "apputils/src/apputilsrepository.h"

#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"

using namespace Tucuxi::Gui::Core::Core;

TargetModel::TargetModel(QObject *parent) :
    AbstractEntityModel(parent),
    _target(),
    _roleNames()
{
    init();
}

TargetModel::TargetModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityModel(undoStack, parent),
    _target(),
    _roleNames()
{
    init();
}

TargetModel::~TargetModel()
{

}

Qt::ItemFlags TargetModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> TargetModel::roleNames() const
{
    return _roleNames;
}

QVariant TargetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case TypeRole:
        return Target::name(_target->getType());
        break;
    case CMinValueRole:
        return _target->getCmin()->value();
        break;
    case CMinUnitRole:
        return _target->getCmin()->unit().name();
        break;
    case CMaxValueRole:
        return _target->getCmax()->value();
        break;
    case CMaxUnitRole:
        return _target->getCmax()->unit().name();
        break;
    case CBestValueRole:
        return _target->getCbest()->value();
        break;
    case CBestUnitRole:
        return _target->getCbest()->unit().name();
        break;
    case TMinValueRole:
        return _target->getTmin()->value();
        break;
    case TMinUnitRole:
        return _target->getTmin()->unit().name();
        break;
    case TMaxValueRole:
        return _target->getTmax()->value();
        break;
    case TMaxUnitRole:
        return _target->getTmax()->unit().name();
        break;
    case TBestValueRole:
        return _target->getTbest()->value();
        break;
    case TBestUnitRole:
        return _target->getTbest()->unit().name();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int TargetModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return !_target ? 0 : 1;
}

void TargetModel::setModelData(Tucuxi::Gui::Core::Target* target)
{
    beginResetModel();
    _target = target;
    endResetModel();
    emit updateStatus();
}

void TargetModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    switch (role) {
    case TypeRole:
        switch (value.toInt()) {
        case 0:
            _target->setType(Tucuxi::Gui::Core::ResidualTarget);
            break;
        case 1:
            _target->setType(Tucuxi::Gui::Core::PeakTarget);
            break;
        case 2:
            _target->setType(Tucuxi::Gui::Core::MeanTarget);
            break;
        default:
            _target->setType(Tucuxi::Gui::Core::UnknownTarget);
            break;
        }
        break;
        return;
    case CMinValueRole:
        _target->getCmin()->setValue(value.toDouble());
        return;
    case CMinUnitRole:
        _target->getCmin()->setUnit(Unit(value.toString()));
        return;
    case CMaxValueRole:
        _target->getCmax()->setValue(value.toDouble());
        return;
    case CMaxUnitRole:
        _target->getCmax()->setUnit(Unit(value.toString()));
        return;
    case CBestValueRole:
        _target->getCbest()->setValue(value.toDouble());
        return;
    case CBestUnitRole:
        _target->getCbest()->setUnit(Unit(value.toString()));
        return;
    case TMinValueRole:
        _target->getTmin()->setValue(value.toDouble());
        return;
    case TMinUnitRole:
        _target->getTmin()->setUnit(Unit(value.toString()));
        return;
    case TMaxValueRole:
        _target->getTmax()->setValue(value.toDouble());
        return;
    case TMaxUnitRole:
        _target->getTmax()->setUnit(Unit(value.toString()));
        return;
    case TBestValueRole:
        _target->getTbest()->setValue(value.toDouble());
        return;
    case TBestUnitRole:
        _target->getTbest()->setUnit(Unit(value.toString()));
        return;
    }
    emit updateStatus();
}

void TargetModel::init()
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
