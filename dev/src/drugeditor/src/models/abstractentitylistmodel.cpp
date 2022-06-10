//@@license@@

#include "abstractentitylistmodel.h"

#include <QUndoStack>

AbstractEntityListModel::AbstractEntityListModel(QObject *parent) :
    AbstractEntityModel(parent)
{

}

AbstractEntityListModel::AbstractEntityListModel(QUndoStack *undoStack, QObject *parent) :
    AbstractEntityModel(undoStack, parent)
{

}

AbstractEntityListModel::~AbstractEntityListModel()
{

}

void AbstractEntityListModel::updateRow(int row, const QVector<int> &roles)
{
    if (row < 0 || row >= rowCount())
        return;

    QModelIndex index = this->index(row, 0);
    emit dataChanged(index, index, roles);
}

bool AbstractEntityListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (row < 0  || row > rowCount())
        return false;
    if (count < 1)
        return false;

    undoStack()->push(new InsertRowsCommand(row, count, this));
    return true;
}

bool AbstractEntityListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (row < 0  || row + count > rowCount())
        return false;
    if (count < 1)
        return false;

    undoStack()->push(new RemoveRowsCommand(row, count, this));
    return true;
}

void AbstractEntityListModel::insertRowsImpl(int at, int count, const Tucuxi::GuiCore::SharedEntitySet &entities)
{
    Q_UNUSED(at);
    Q_UNUSED(count);
    Q_UNUSED(entities);
}

Tucuxi::GuiCore::SharedEntitySet AbstractEntityListModel::removeRowsImpl(int at, int count)
{
    Q_UNUSED(at);
    Q_UNUSED(count);
    return Tucuxi::GuiCore::SharedEntitySet();
}

InsertRowsCommand::InsertRowsCommand(int row, int count, AbstractEntityListModel *model) :
    QUndoCommand(),
    _model(model),
    _at(row),
    _count(count)
{
    Q_ASSERT(model);
    Q_ASSERT(row >= 0 && row <= _model->rowCount());
    Q_ASSERT(count > 0);
}

InsertRowsCommand::~InsertRowsCommand()
{

}

void InsertRowsCommand::redo()
{
    _model->beginInsertRows(QModelIndex(), _at, _at + _count - 1);
    _model->insertRowsImpl(_at, _count);
    _model->endInsertRows();
}

void InsertRowsCommand::undo()
{
    Tucuxi::GuiCore::SharedEntitySet removed;

    _model->beginRemoveRows(QModelIndex(), _at, _at + _count - 1);
    removed << _model->removeRowsImpl(_at, _count);
    _model->endRemoveRows();

    //ToDo: Delete the removed entities from the DB ?
}

RemoveRowsCommand::RemoveRowsCommand(int row, int count, AbstractEntityListModel *model) :
    QUndoCommand(),
    _model(model),
    _removed(),
    _at(row),
    _count(count)
{
    Q_ASSERT(model);
    Q_ASSERT(row >= 0 && row + count <= _model->rowCount());
    Q_ASSERT(count > 0);
}

RemoveRowsCommand::~RemoveRowsCommand()
{
    // ToDo: Delete the _removed entities from the DB ?
}

void RemoveRowsCommand::redo()
{
    _model->beginRemoveRows(QModelIndex(), _at, _at + _count - 1);
    _removed << _model->removeRowsImpl(_at, _count);
    _model->endRemoveRows();
}

void RemoveRowsCommand::undo()
{
    _model->beginInsertRows(QModelIndex(), _at, _at + _count - 1);
    _model->insertRowsImpl(_at, _count, _removed);
    _model->endInsertRows();

    _removed.clear();
}
