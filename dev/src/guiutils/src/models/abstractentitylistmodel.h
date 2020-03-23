#ifndef ABSTRACTENTITYLISTMODEL_H
#define ABSTRACTENTITYLISTMODEL_H

#include "abstractentitymodel.h"

#include <QUndoCommand>

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
    virtual void insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities = ezechiel::core::SharedEntitySet());
    virtual ezechiel::core::SharedEntitySet removeRowsImpl(int at, int count);

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
    ezechiel::core::SharedEntitySet _removed;
    int _at;
    int _count;
};

#endif // ABSTRACTENTITYLISTMODEL_H
