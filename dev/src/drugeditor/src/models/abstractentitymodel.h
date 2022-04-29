//@@license@@

#ifndef ABSTRACTENTITYMODEL_H
#define ABSTRACTENTITYMODEL_H

#include "core/dal/entity.h"

#include <QAbstractListModel>
#include <QUndoCommand>

class AbstractEntityModel : public QAbstractListModel
{
    Q_OBJECT

    friend class SetDataCommand;

public:
    explicit AbstractEntityModel(QObject *parent = 0);
    explicit AbstractEntityModel(QUndoStack *undoStack, QObject *parent = 0);
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

#endif // ABSTRACTENTITYMODEL_H
