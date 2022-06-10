//@@license@@

#include "covariatemodel.h"

#include "core/utils/units.h"

CovariateModel::CovariateModel(QObject *parent) :
    AbstractEntityModel(parent),
    _covariate(),
    _roleNames()
{
    init();
}

CovariateModel::CovariateModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityModel(undoStack, parent),
    _covariate(),
    _roleNames()
{
    init();
}

CovariateModel::~CovariateModel()
{

}

Qt::ItemFlags CovariateModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> CovariateModel::roleNames() const
{
    return _roleNames;
}

QVariant CovariateModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return _covariate->getName();
        break;
    case DescriptionRole:
        return _covariate->getDescription();
        break;
    case ValueRole:
        return _covariate->getQuantity()->value();
        break;
    case UnitRole:
        return _covariate->getQuantity()->unit().name();
        break;
    case TypeRole:
        return QMetaType::typeName(_covariate->getType());
        break;
    case DateRole:
        return _covariate->getDate();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int CovariateModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return !_covariate ? 0 : 1;
}

void CovariateModel::setModelData(Tucuxi::Gui::Core::PatientVariate* covariate)
{
    beginResetModel();
    _covariate = covariate;
    endResetModel();
    emit updateStatus();
}

void CovariateModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    switch (role) {
    case NameRole:
        _covariate->setName(value.toString());
        break;
    case DescriptionRole:
        _covariate->setDescription(value.toString());
        break;
    case ValueRole:
        _covariate->getQuantity()->setValue(value.toDouble());
        break;
    case UnitRole:
        _covariate->getQuantity()->setUnit(Tucuxi::Gui::Core::Unit(value.toString()));
        break;
    case TypeRole:
        _covariate->setType(static_cast<QMetaType::Type>(QMetaType::type(qPrintable(value.toString()))));
        break;
    case DateRole:
        _covariate->setDate(value.toDateTime());
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void CovariateModel::init()
{
    _roleNames[NameRole]        = "name";
    _roleNames[DescriptionRole] = "description";
    _roleNames[ValueRole]       = "value";
    _roleNames[UnitRole]        = "unit";
    _roleNames[TypeRole]        = "type";
    _roleNames[DateRole]        = "date";
}
