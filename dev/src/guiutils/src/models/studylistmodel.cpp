//@@license@@

#include "studylistmodel.h"

StudyListModel::StudyListModel(QObject *parent) :
    AbstractEntityListModel(parent),
    _studies(),
    _roleNames()
{
    init();
}

StudyListModel::StudyListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityListModel(undoStack, parent),
    _studies(),
    _roleNames()
{
    init();
}

StudyListModel::~StudyListModel()
{

}

QHash<int, QByteArray> StudyListModel::roleNames() const
{
    return _roleNames;
}

QVariant StudyListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case IdRole:
        return _studies.at(index.row()).id;
        break;
    case NameRole:
        return _studies.at(index.row()).name;
        break;
    case DescriptionRole:
        return _studies.at(index.row()).description;
        break;
    }

    Q_ASSERT(false);
    return QVariant();
}

int StudyListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _studies.count();
}

bool StudyListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    Q_UNIMPLEMENTED();
    return false;
}

bool StudyListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

bool StudyListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    Q_UNIMPLEMENTED();
    return false;
}

void StudyListModel::setModelData(const QList<Tucuxi::GuiCore::Descriptor> &studies)
{
    beginResetModel();
    _studies = studies;
    endResetModel();
    emit updateStatus();
}

void StudyListModel::init()
{
    _roleNames[IdRole]          = "id";
    _roleNames[NameRole]        = "name";
    _roleNames[DescriptionRole] = "description";
}
