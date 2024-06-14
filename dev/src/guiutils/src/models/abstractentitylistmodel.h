/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef ABSTRACTENTITYLISTMODEL_H
#define ABSTRACTENTITYLISTMODEL_H

#include "abstractentitymodel.h"

#include <QUndoCommand>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class AbstractEntityListModel : public AbstractEntityModel
{
    Q_OBJECT

    friend class InsertRowsCommand;
    friend class RemoveRowsCommand;

public:
    explicit AbstractEntityListModel(QObject *parent = nullptr);
    explicit AbstractEntityListModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~AbstractEntityListModel() Q_DECL_OVERRIDE = 0;

    Q_INVOKABLE virtual void updateRow(int row, const QVector<int> &roles = QVector<int>());

    Q_INVOKABLE virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

private:
    virtual void insertRowsImpl(int at, int count, const Tucuxi::Gui::Core::SharedEntitySet &entities = Tucuxi::Gui::Core::SharedEntitySet());
    virtual Tucuxi::Gui::Core::SharedEntitySet removeRowsImpl(int at, int count);

};

class InsertRowsCommand : public QUndoCommand
{

public:
    InsertRowsCommand(int row, int count, AbstractEntityListModel *model);
    virtual ~InsertRowsCommand();

    virtual void redo() Q_DECL_OVERRIDE;
    virtual void undo() Q_DECL_OVERRIDE;

private:
    AbstractEntityListModel *_model;
    int _at;
    int _count;
};

class RemoveRowsCommand : public QUndoCommand
{

public:
    RemoveRowsCommand(int row, int count, AbstractEntityListModel *model);
    virtual ~RemoveRowsCommand();

    virtual void redo() Q_DECL_OVERRIDE;
    virtual void undo() Q_DECL_OVERRIDE;

private:
    AbstractEntityListModel *_model;
    Tucuxi::Gui::Core::SharedEntitySet _removed;
    int _at;
    int _count;
};

}
}
}

#endif // ABSTRACTENTITYLISTMODEL_H
