//@@license@@

#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include "QUndoCommand"
#include "core/dal/entity.h"
#include"../guiutils/src/models/abstractentitylistmodel.h"

#include "../guiutils/src/models/sortfilterentitylistproxymodel.h"

#include "core/utils/ezutils.h"

#include "core/dal/drug/translatablestring.h"

#define EDIT_TEXT_ID 1
#define CHANGE_VIEW_INDEX 2

class UndoRedoController;


class EditTextCommand : public QUndoCommand
{
public:

    EditTextCommand(ezechiel::GuiCore::Entity *entity,QString propertyName,QVariant newValue, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    int id() const Q_DECL_OVERRIDE { return EDIT_TEXT_ID; }


private:
    ezechiel::GuiCore::Entity *_entity;
    QString _propertyName;
    QVariant _oldValue, _newValue;
};

class AddCommand : public QUndoCommand
{
public:

    //    AddCommand(SortFilterEntityListProxyModel *model, QObject *view, QUndoCommand *parent = 0);
    AddCommand(AbstractEntityListModel *model, QObject *view, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    AbstractEntityListModel *_model;
    QObject *_view;
};

class RemoveCommand : public QUndoCommand
{
public:

    RemoveCommand(AbstractEntityListModel *model, QObject *view, int index, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    AbstractEntityListModel *_model;
    QObject *_view;
    int _index;
    QMap<int,QVariant> _itemData;
};




class ChangeViewIndexCommand : public QUndoCommand
{
public:
    ChangeViewIndexCommand(QObject *containingView, int newIndex, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
    bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE { return CHANGE_VIEW_INDEX; }


private:
    QObject *_containingView;
    int _oldIndex, _newIndex;
    //    bool _isCurrentIndex;
    QString _indexName;
};

/**
 * @brief The EditCommentCommand class
 * @param reference the lang if the comment changes, the comment otherwise
 */
class EditCommentCommand : public QUndoCommand
{
public:
    EditCommentCommand(ezechiel::GuiCore::TranslatableString *comments, QString newValue,QString reference,void (UndoRedoController::*signal)(ezechiel::GuiCore::TranslatableString*),UndoRedoController *signalClass,bool isLangChanging = false,QUndoCommand *parent = 0);
    EditCommentCommand(ezechiel::GuiCore::TranslatableString *comments, QString newValue,QString reference,void (UndoRedoController::*signal)(),UndoRedoController *signalClass,bool isLangChanging = false,QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:

    ezechiel::GuiCore::TranslatableString *_comments;
    QString _oldValue, _newValue, _reference;
    bool _isLangChanging;
    void (UndoRedoController::*_signal)(ezechiel::GuiCore::TranslatableString*);
    void (UndoRedoController::*_signalEntity)();
    UndoRedoController *_signalClass;

};
/**
 * @brief The AddCommentCommand class adds a comment if index == -1, removes one otherwise. It sends a signal in both cases once the job's done
 */
class AddCommentCommand : public QUndoCommand
{
public:
    AddCommentCommand(ezechiel::GuiCore::TranslatableString *comments,int index,void (UndoRedoController::*signal)(ezechiel::GuiCore::TranslatableString*),UndoRedoController *signalClass,QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:



    ezechiel::GuiCore::TranslatableString *_comments;
    QString _lang, _comment;
    int _index;
    void (UndoRedoController::*_signal)(ezechiel::GuiCore::TranslatableString*);
    UndoRedoController *_signalClass;
};

class EditObjCommand : public QUndoCommand
{
public:
    EditObjCommand(QObject *obj, QString property, QVariant newValue,void (UndoRedoController::*signal)(QObject*) = nullptr,UndoRedoController *signalClass = nullptr , QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:

    QObject *_obj;
    QString _property;
    QVariant _oldValue, _newValue;
    void (UndoRedoController::*_signal)(QObject*);
    UndoRedoController *_signalClass;

};
template <typename T>
class AddObjCommand : public QUndoCommand
{
public:
    AddObjCommand(QList<T*> *objList, int index, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:

    T *_obj;
    QList<T*> * _objList;
    int _index;



};

class RemoveObjCommand : public QUndoCommand
{

public:
    RemoveObjCommand(QObject *objList, int index, QUndoCommand *parent = 0);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:

    QObject *_obj;
    QObject*  _objList;
    int _index,_size;

};



#endif // UNDOCOMMANDS_H
