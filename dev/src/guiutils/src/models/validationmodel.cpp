//@@license@@

#include "validationmodel.h"

#include "core/dal/prediction.h"

using namespace Tucuxi::Gui::Admin;
using namespace Tucuxi::Gui::GuiUtils;

ValidationModel::ValidationModel(QObject *parent) :
    AbstractEntityModel(parent),
    _analysis(),
    _roleNames()
{
    init();
}

ValidationModel::ValidationModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityModel(undoStack, parent),
    _analysis(),
    _roleNames()
{
    init();
}

ValidationModel::~ValidationModel()
{

}

Qt::ItemFlags ValidationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ValidationModel::roleNames() const
{
    return _roleNames;
}

QVariant ValidationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case ExpectednessRole:
        return _analysis->getExpectedness();
        break;
    case SuitabilityRole:
        return _analysis->getSuitability();
        break;
    case PredictionRole:
        return _analysis->getPrediction();
        break;
    case RemonitoringRole:
        return _analysis->getRemonitoring();
        break;
    case WarningRole:
        return _analysis->getWarning();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int ValidationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return !_analysis ? 0 : 1;
}

void ValidationModel::setModelData(InterpretationAnalysis* analysis)
{
    beginResetModel();
    _analysis = analysis;
    endResetModel();
    emit updateStatus();
}

void ValidationModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    switch (role) {
    case ExpectednessRole:
        _analysis->setExpectedness(value.toString());
        break;
    case SuitabilityRole:
        _analysis->setSuitability(value.toString());
        break;
    case PredictionRole:
        _analysis->setPrediction(value.toString());
        break;
    case RemonitoringRole:
        _analysis->setRemonitoring(value.toString());
        break;
    case WarningRole:
        _analysis->setWarning(value.toString());
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void ValidationModel::init()
{
    _roleNames[ExpectednessRole] = "expectedness";
    _roleNames[SuitabilityRole] = "suitability";
    _roleNames[PredictionRole] = "prediction";
    _roleNames[RemonitoringRole] = "remonitoring";
    _roleNames[WarningRole] = "warning";
}
