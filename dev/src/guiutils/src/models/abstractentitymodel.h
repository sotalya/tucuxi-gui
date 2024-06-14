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


#ifndef ABSTRACTENTITYMODEL_H
#define ABSTRACTENTITYMODEL_H

#include "core/dal/entity.h"

#include <QAbstractListModel>
#include <QUndoCommand>

namespace Tucuxi {
namespace Gui {
namespace GuiUtils {

class AbstractEntityModel : public QAbstractListModel
{
    Q_OBJECT

    friend class SetDataCommand;

public:
    explicit AbstractEntityModel(QObject *parent = nullptr);
    explicit AbstractEntityModel(QUndoStack *undoStack, QObject *parent = nullptr);
    virtual ~AbstractEntityModel() Q_DECL_OVERRIDE = 0;

    Q_INVOKABLE virtual bool clearModel();

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    QUndoStack *undoStack() const;
    void setUndoStack(QUndoStack *undoStack);

private:
    virtual void setDataImpl(const QModelIndex &index, const QVariant &value, int role);

    void emitDataChanged(const QModelIndex &index, const QVector<int> &roles = QVector<int>());

    QUndoStack *_undoStack;
};

class SetDataCommand : public QUndoCommand
{

public:
    SetDataCommand(const QModelIndex &index, const QVariant &value, int role, AbstractEntityModel *model);
    virtual ~SetDataCommand();

    virtual void redo() Q_DECL_OVERRIDE;
    virtual void undo() Q_DECL_OVERRIDE;

private:
    AbstractEntityModel *_model;
    QVariant _oldValue;
    QVariant _newValue;
    int _role;
    int _row;
    int _col;
};

}
}
}

#endif // ABSTRACTENTITYMODEL_H
