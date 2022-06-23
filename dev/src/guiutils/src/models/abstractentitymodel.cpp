//@@license@@

#include "abstractentitymodel.h"

#include <QUndoStack>

using namespace Tucuxi::Gui::GuiUtils;

AbstractEntityModel::AbstractEntityModel(QObject *parent) :
    QAbstractListModel(parent),
    _undoStack(new QUndoStack(this))
{

}

AbstractEntityModel::AbstractEntityModel(QUndoStack *undoStack, QObject *parent) :
    QAbstractListModel(parent),
    _undoStack(undoStack)
{
    Q_ASSERT(undoStack);
}

AbstractEntityModel::~AbstractEntityModel()
{

}

bool AbstractEntityModel::clearModel()
{
    if (rowCount() > 0)
        removeRows(0, rowCount());

    _undoStack->clear();

    return true;
}

bool AbstractEntityModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (!value.isValid())
        return false;
    if (!roleNames().contains(role))
        return false;
    if (index.data(role) == value)
        return false;

    _undoStack->push(new SetDataCommand(index, value, role, this));
    return true;
}

QUndoStack *AbstractEntityModel::undoStack() const
{
    return _undoStack;
}

void AbstractEntityModel::setUndoStack(QUndoStack *undoStack)
{
    Q_ASSERT(undoStack);

    if (_undoStack->parent() == this)
        delete _undoStack;

    _undoStack = undoStack;
}

void AbstractEntityModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
}

void AbstractEntityModel::emitDataChanged(const QModelIndex &index, const QVector<int> &roles)
{
    emit dataChanged(index, index, roles);
}

SetDataCommand::SetDataCommand(const QModelIndex &index, const QVariant &value, int role, AbstractEntityModel *model) :
    QUndoCommand(),
    _model(model),
    _oldValue(),
    _newValue(value),
    _role(role),
    _row(-1),
    _col(-1)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(value.isValid());
    Q_ASSERT(model);
    Q_ASSERT(model->roleNames().contains(role));

    _oldValue = index.data(role);
    _row = index.row();
    _col = index.column();
}

SetDataCommand::~SetDataCommand()
{

}

void SetDataCommand::redo()
{
    QModelIndex index = _model->index(_row, _col);
    _model->setDataImpl(index, _newValue, _role);
    _model->emitDataChanged(index, QVector<int>() << _role);
}

void SetDataCommand::undo()
{
    QModelIndex index = _model->index(_row, _col);
    _model->setDataImpl(index, _oldValue, _role);
    _model->emitDataChanged(index, QVector<int>() << _role);
}
