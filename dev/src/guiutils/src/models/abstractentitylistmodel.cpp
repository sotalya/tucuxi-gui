/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "abstractentitylistmodel.h"

#include <QUndoStack>

using namespace Tucuxi::Gui::GuiUtils;

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

void AbstractEntityListModel::insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities)
{
    Q_UNUSED(at);
    Q_UNUSED(count);
    Q_UNUSED(entities);
}

Tucuxi::Gui::Core::SharedEntitySet AbstractEntityListModel::removeRowsImpl(int at, int count)
{
    Q_UNUSED(at);
    Q_UNUSED(count);
    return Tucuxi::Gui::Core::SharedEntitySet();
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
    Tucuxi::Gui::Core::SharedEntitySet removed;

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
