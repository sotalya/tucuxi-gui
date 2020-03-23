#include "covariatelistmodel.h"
#include "corefactory.h"
#include "stdadminrepository.h"
#include "corerepository.h"
#include "logging.h"
#include "errors_guiutils.h"

using namespace ezechiel::core;

CovariateListModel::CovariateListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _covariates(0),
    _drugVariate(),
    _roleNames()
{
    init();
}

CovariateListModel::CovariateListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _covariates(0),
    _drugVariate(),
    _roleNames()
{
    init();
}

CovariateListModel::~CovariateListModel()
{

}

Qt::ItemFlags CovariateListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> CovariateListModel::roleNames() const
{
    return _roleNames;
}

QVariant CovariateListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return _covariates->at(index.row())->getName();
        break;
    case DescriptionRole:
        return _covariates->at(index.row())->getDescription();
        break;
    case ValueRole:
        return _covariates->at(index.row())->getQuantity()->value();
        break;
    case UnitRole:
        return _covariates->at(index.row())->getQuantity()->unit().name();
        break;
    case TypeRole:
        return QMetaType::typeName(_covariates->at(index.row())->getType());
        break;
    case DateRole:
        return _covariates->at(index.row())->getDate();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int CovariateListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _covariates->getList().count();
}

void CovariateListModel::setModelData(ezechiel::core::PatientVariateList *covariates)
{
    beginResetModel();
    _covariates = covariates;
    endResetModel();
    emit updateStatus();
}

void CovariateListModel::setModelData(DrugVariate* drugVariate)
{
    _drugVariate = drugVariate;
    emit updateStatus();
}

void CovariateListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case NameRole:
        _covariates->at(row)->setName(value.toString());
        break;
    case DescriptionRole:
        _covariates->at(row)->setDescription(value.toString());
        break;
    case ValueRole:
        _covariates->at(row)->getQuantity()->setValue(value.toDouble());
        break;
    case UnitRole:
        _covariates->at(row)->getQuantity()->setUnit(Unit(value.toString()));
        break;
    case TypeRole:
        _covariates->at(row)->setType(static_cast<QMetaType::Type>(QMetaType::type(qPrintable(value.toString()))));
        break;
    case DateRole:
        _covariates->at(row)->setDate(value.toDateTime());
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void CovariateListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);
    Q_ASSERT(_drugVariate);

    ezechiel::core::PatientVariate* covariate;
    for(int i = at, j = 0; i < at + count; i++, j++) {

        //Setup or retrieve the dosage

        // YTA: We should have a copy constructor
        if (entities.isEmpty()) {
            covariate = ezechiel::core::CoreFactory::createEntity<ezechiel::core::PatientVariate>(ABSTRACTREPO);
            covariate->setName(_drugVariate->getName());
            covariate->getQuantity()->setUnit(_drugVariate->getQuantity()->unit());
            covariate->setDescription(_drugVariate->getDescription());
            covariate->setDescriptor(_drugVariate->getDescriptor());
            covariate->setType(_drugVariate->getType());
            covariate->getQuantity()->setValue(_drugVariate->getQuantity()->value());
            covariate->setDate(QDateTime::currentDateTime());
        } else {
            covariate = static_cast<ezechiel::core::PatientVariate*>(entities.at(j));
        }

        _covariates->getList().insert(i, covariate);
    }
    emit updateStatus();
}

ezechiel::core::SharedEntitySet CovariateListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    ezechiel::core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _covariates->getList().takeAt(at);

    emit updateStatus();
    return removed;
}

void CovariateListModel::init()
{
    _roleNames[NameRole]        = "name";
    _roleNames[DescriptionRole] = "description";
    _roleNames[ValueRole]       = "value";
    _roleNames[UnitRole]        = "unit";
    _roleNames[TypeRole]        = "type";
    _roleNames[DateRole]        = "date";
}
