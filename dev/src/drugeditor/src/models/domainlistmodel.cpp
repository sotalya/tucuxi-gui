//@@license@@

#include "domainlistmodel.h"

DomainListModel::DomainListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _domains(),
    _roleNames()
{
    init();
}

DomainListModel::DomainListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _domains(),
    _roleNames()
{
    init();
}

DomainListModel::~DomainListModel()
{

}

QHash<int, QByteArray> DomainListModel::roleNames() const
{
    return _roleNames;
}

QVariant DomainListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case IdRole:
        return _domains.at(index.row()).id;
        break;
    case NameRole:
        return _domains.at(index.row()).name;
        break;
    case DescriptionRole:
        return _domains.at(index.row()).description;
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int DomainListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _domains.count();
}

bool DomainListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return false;
}

bool DomainListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

bool DomainListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

void DomainListModel::setModelData(const QList<ezechiel::GuiCore::Descriptor> &domains)
{
    beginResetModel();
    _domains = domains;
    endResetModel();
    emit updateStatus();
}

void DomainListModel::init()
{
    _roleNames[IdRole]          = "id";
    _roleNames[NameRole]        = "name";
    _roleNames[DescriptionRole] = "description";
}
