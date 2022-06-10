//@@license@@

#include "measurelistmodel.h"
#include "admin/src/adminfactory.h"
#include "admin/src/stdadminrepository.h"
#include "core/corerepository.h"
#include "apputils/src/apputilsrepository.h"
#include "core/utils/logging.h"
#include "errors_guiutils.h"
#include "core/dal/drug/concentrations.h"


MeasureListModel::MeasureListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _measures(0),
    _drug(),
    _defaultUnit(Tucuxi::Gui::Core::Unit("mg/l")),
    _roleNames()
{
    init();
}

MeasureListModel::MeasureListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _measures(0),
    _drug(),
    _defaultUnit(Tucuxi::Gui::Core::Unit("mg/l")),
    _roleNames()
{
    init();
}

MeasureListModel::~MeasureListModel()
{

}

Qt::ItemFlags MeasureListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> MeasureListModel::roleNames() const
{
    return _roleNames;
}

QVariant MeasureListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case SampleIdRole:
        return static_cast<Measure*>(_measures->at(index.row()))->sampleID();
        break;
    case ValueRole:
        return _measures->at(index.row())->getConcentration()->value();
        break;
    case UnitRole:
        return _measures->at(index.row())->getConcentration()->unit().name();
        break;
    case SampleDateRole:
        return _measures->at(index.row())->getMoment();
        break;
    case ArrivalDateRole:
        return static_cast<Measure*>(_measures->at(index.row()))->arrivalDate();
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int MeasureListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (_measures) {
        return _measures->count();
    }
}

void MeasureListModel::setModelData(QList<Tucuxi::Gui::Core::CoreMeasure *> *measures)
{
    beginResetModel();
    _measures = measures;
    foreach (Tucuxi::Gui::Core::CoreMeasure* cm, *measures) {
        cm->getConcentration()->convert(_defaultUnit);
    }
    endResetModel();
    emit updateStatus();
}

void MeasureListModel::setModelData(Tucuxi::Gui::Core::DrugModel* drug)
{
    _drug = drug;
    emit updateStatus();
}

void MeasureListModel::setDefaultUnit(QString unitname) {
    _defaultUnit = Tucuxi::Gui::Core::Unit(unitname);
}

void MeasureListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    int row = index.row();
    Q_ASSERT(row < rowCount());

    switch (role) {
    case SampleIdRole:
        static_cast<Measure*>(_measures->at(row))->sampleID(value.toString());
        break;
    case UnitRole:
        _measures->at(row)->getConcentration()->convert(Tucuxi::Gui::Core::Unit(value.toString()));
        break;
    case ValueRole:
        _measures->at(row)->getConcentration()->setValue(value.toDouble());
        break;
    case SampleDateRole:
        _measures->at(row)->setMoment(value.toDateTime());
        break;
    case ArrivalDateRole:
        static_cast<Measure*>(_measures->at(row))->arrivalDate(value.toDateTime());
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void MeasureListModel::insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);
    Q_ASSERT(_drug);
    Q_ASSERT(_drug->getConcentrations());

    Measure * measure;
    for(int i = at, j = 0; i < at + count; i++, j++) {

        //Setup or retrieve the dosage
        if (entities.isEmpty()) {
            measure = AdminFactory::createEntity<Measure>(ABSTRACTREPO);
            static_cast<Measure*>(measure)->getConcentration()->setUnit(_defaultUnit);
            static_cast<Measure*>(measure)->setMoment(QDateTime::currentDateTime());
            static_cast<Measure*>(measure)->arrivalDate(QDateTime::currentDateTime());
        } else {
            measure = static_cast<Measure*>(entities.at(j));
        }

        _measures->insert(i, measure);
    }
    emit updateStatus();
}

Tucuxi::Gui::Core::SharedEntitySet MeasureListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);

    Tucuxi::Gui::Core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
        removed << _measures->takeAt(at);

    emit updateStatus();
    return removed;
}

void MeasureListModel::init()
{
    _roleNames[SampleIdRole]    = "sampleId";
    _roleNames[ValueRole]       = "value";
    _roleNames[UnitRole]        = "unit";
    _roleNames[SampleDateRole]  = "sampleDate";
    _roleNames[ArrivalDateRole] = "arrivalDate";
}
