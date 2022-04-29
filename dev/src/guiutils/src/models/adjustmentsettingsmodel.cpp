//@@license@@

#include "adjustmentsettingsmodel.h"

AdjustmentSettingsModel::AdjustmentSettingsModel(QObject *parent) :
    AbstractEntityModel(parent),
    _date(QDateTime(QDate::currentDate(), QTime::currentTime())),
    _valid(false),
    _roleNames()
{
    init();
}

AdjustmentSettingsModel::AdjustmentSettingsModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityModel(undoStack, parent),
    _date(QDateTime(QDate::currentDate(), QTime::currentTime())),
    _valid(false),
    _roleNames()
{
    init();
}

AdjustmentSettingsModel::~AdjustmentSettingsModel()
{

}

void AdjustmentSettingsModel::setModelData(QDateTime time)
{
    beginResetModel();
    _date = time;
    endResetModel();
    emit updateStatus();
}

bool AdjustmentSettingsModel::clearModel()
{
    beginResetModel();
    _date  = QDateTime(QDate::currentDate(), QTime::currentTime());
    _valid = false;
    endResetModel();
    emit updateStatus();
    return true;
}

Qt::ItemFlags AdjustmentSettingsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return AbstractEntityModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> AdjustmentSettingsModel::roleNames() const
{
    return _roleNames;
}

QVariant AdjustmentSettingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case DateRole:
        return _date;
        break;
    case ValidRole:
        return _valid;
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int AdjustmentSettingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void AdjustmentSettingsModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    switch (role) {
    case DateRole:
        _date = value.toDateTime();
        emit dateChanged(_date);
        break;
    case ValidRole:
        _valid = value.toBool();
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    emit updateStatus();
}

void AdjustmentSettingsModel::init()
{
    _roleNames[DateRole]  = "date";
    _roleNames[ValidRole] = "valid";
}
