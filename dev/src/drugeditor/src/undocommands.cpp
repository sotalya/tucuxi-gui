//@@license@@

#include "undocommands.h"

#include "undoredocontroller.h"

#include "core/dal/drug/doses.h"
#include "core/dal/drug/intervals.h"
#include "core/dal/drug/infusions.h"
#include "core/utils/connect.h"

EditTextCommand::EditTextCommand(Tucuxi::Gui::Core::Entity *entity,QString propertyName,QVariant newValue, QUndoCommand *parent):
    QUndoCommand(parent),
    _newValue(newValue),
    _propertyName(propertyName),
    _entity(entity)
{
    _oldValue = (_entity->property(qPrintable(_propertyName)));



}

void EditTextCommand::undo()
{
    QUndoCommand::undo();
    _entity->setProperty(qPrintable(_propertyName),_oldValue);

}

void EditTextCommand::redo()
{
    QUndoCommand::redo();
    _entity->setProperty(qPrintable(_propertyName),_newValue);
}

bool EditTextCommand::mergeWith(const QUndoCommand *command)
{

    const EditTextCommand *editTextCommand = static_cast<const EditTextCommand *>(command);
    if(_propertyName == "date")
        return false;
    if(_propertyName != editTextCommand->_propertyName ||
            _entity != editTextCommand->_entity)
        return false;

    _newValue = editTextCommand->_newValue;

    return true;

}

AddCommand::AddCommand(AbstractEntityListModel *model,QObject *view, QUndoCommand *parent):
    _model(model),
    _view(view)
{

}

void AddCommand::undo()
{
    QUndoCommand::undo();
    _model->removeRow(_model->rowCount() - 1);
    _view->setProperty("currentIndex", _model->rowCount() - 1);

}

void AddCommand::redo()
{
    QUndoCommand::redo();
    _model->insertRow(_model->rowCount());
    _view->setProperty("currentIndex", _model->rowCount() - 1);
}




RemoveCommand::RemoveCommand(AbstractEntityListModel *model, QObject *view, int index, QUndoCommand *parent):
    QUndoCommand(parent),
    _model(model),
    _index(index),
    _view(view)
{
    QMap<int,QVariant> itemData = _model->itemData(_model->index(_index,0));
    _itemData = itemData;

}

void RemoveCommand::undo()
{
    QUndoCommand::undo();
    _model->insertRow(_index);
    _model->setItemData(_model->index(_index,0),_itemData);
    _view->setProperty("currentIndex", _index);
}

void RemoveCommand::redo()
{
    QUndoCommand::redo();
    _model->removeRow(_index);
    int index = _index;
    if(index >= _model->rowCount())
        index = _model->rowCount() - 1;

    _view->setProperty("currentIndex", index);
}


ChangeViewIndexCommand::ChangeViewIndexCommand(QObject *containingView, int newIndex, QUndoCommand *parent):
    QUndoCommand(parent),
    _newIndex(newIndex),
    _containingView(containingView)
{
    _indexName = "index";
    QVariant index = _containingView->property(qPrintable(_indexName));
    if(!index.isValid()) {
        _indexName = "currentIndex";
        index = _containingView->property(qPrintable(_indexName));
        Q_ASSERT(index.isValid());
    }

    _oldIndex = index.toInt();

}

void ChangeViewIndexCommand::undo()
{
    QUndoCommand::undo();
    _containingView->setProperty(qPrintable(_indexName),_oldIndex);

}

void ChangeViewIndexCommand::redo()
{
    QUndoCommand::redo();
    _containingView->setProperty(qPrintable(_indexName),_newIndex);

}

bool ChangeViewIndexCommand::mergeWith(const QUndoCommand *command)
{
    const ChangeViewIndexCommand *cviCmd = static_cast<const ChangeViewIndexCommand *>(command);

    if(_containingView != cviCmd->_containingView)
        return false;

    _newIndex = cviCmd->_newIndex;

    return true;
}

EditCommentCommand::EditCommentCommand(Tucuxi::Gui::Core::TranslatableString *comments, QString newValue,QString reference,void (UndoRedoController::*signal)(Tucuxi::Gui::Core::TranslatableString*),UndoRedoController *signalClass,bool isLangChanging,QUndoCommand *parent) :
    QUndoCommand(parent),
    _comments(comments),
    _newValue(newValue),
    _reference(reference),
    _signal(signal),
    _signalEntity(nullptr),
    _signalClass(signalClass),
    _isLangChanging(isLangChanging)
{
    if(_isLangChanging) {
        _oldValue = comments->key(reference);
    } else {
        _oldValue = comments->value(reference);
    }
}
EditCommentCommand::EditCommentCommand(Tucuxi::Gui::Core::TranslatableString *comments, QString newValue,QString reference,void (UndoRedoController::*signal)(),UndoRedoController *signalClass,bool isLangChanging,QUndoCommand *parent) :
    QUndoCommand(parent),
    _comments(comments),
    _newValue(newValue),
    _reference(reference),
    _signal(nullptr),
    _signalEntity(signal),
    _signalClass(signalClass),
    _isLangChanging(isLangChanging)
{
    if(_isLangChanging) {
        _oldValue = comments->key(reference);
    } else {
        _oldValue = comments->value(reference);
    }
}

void EditCommentCommand::undo()
{
    if(_isLangChanging) {
        _comments->remove(_newValue);
        _comments->insert(_oldValue,_reference);
    } else {
        _comments->remove(_reference);
        _comments->insert(_reference,_oldValue);
    }

    if(_signal != nullptr && _signalClass != nullptr)
        (_signalClass->*_signal)(_comments);
    else if (_signalEntity != nullptr && _signalClass != nullptr)
        (_signalClass->*_signal)(_comments);
}

void EditCommentCommand::redo()
{
    if(_isLangChanging) {
        _comments->remove(_oldValue);
        _comments->insert(_newValue,_reference);
    } else {
        _comments->remove(_reference);
        _comments->insert(_reference,_newValue);
    }

    if(_signal != nullptr && _signalClass != nullptr)
        (_signalClass->*_signal)(_comments);
}


AddCommentCommand::AddCommentCommand(Tucuxi::Gui::Core::TranslatableString *comments, int index, void (UndoRedoController::*signal)(Tucuxi::Gui::Core::TranslatableString*),UndoRedoController *signalClass,QUndoCommand *parent) :
    QUndoCommand(parent),
    _comments(comments),
    _index(index),
    _signal(signal),
    _signalClass(signalClass)
{
    if(index > -1 && index < _comments->size()) {
        _lang = _comments->keys().at(index);
        _comment = _comments->value(_lang);
    }
}

void AddCommentCommand::undo()
{

     if(_index > -1 && _index < _comment.size()) {

         _comments->insert(_lang,_comment);

     } else {
         if(_comments->contains(""))
             _comments->remove("");

     }

     if(_signal != nullptr && _signalClass != nullptr)
         (_signalClass->*_signal)(_comments);

}

void AddCommentCommand::redo()
{
    if(_index > -1 && _index < _comment.size()) {

        _comments->take(_lang);

    } else {
        if(!_comments->contains(""))
            _comments->insert("","");
    }

    if(_signal != nullptr && _signalClass != nullptr)
        (_signalClass->*_signal)(_comments);
}

EditObjCommand::EditObjCommand(QObject *obj, QString property, QVariant newValue,void (UndoRedoController::* signal)(QObject*) ,UndoRedoController *signalClass , QUndoCommand *parent) :
    QUndoCommand(parent),
    _obj(obj),
    _property(property),
    _newValue(newValue),
    _signal(signal),
    _signalClass(signalClass)
{

    _oldValue = _obj->property(qPrintable(_property));


}

void EditObjCommand::undo()
{
    _obj->setProperty(qPrintable(_property),_oldValue);

    if(_signal != nullptr && _signalClass != nullptr) {
          (_signalClass->*_signal)(_obj);
    }

}

void EditObjCommand::redo()
{
    (_obj->setProperty(qPrintable(_property),_newValue));
    QVariant avar = _obj->property(qPrintable(_property));
    if(_signal != nullptr && _signalClass != nullptr) {
          (_signalClass->*_signal)(_obj);
    }

}
template<typename T>
AddObjCommand<T>::AddObjCommand(QList<T*> *objbList, int index, QUndoCommand *parent) :
    QUndoCommand(parent),
    _objList(objbList),
    _index(index),
    _obj(nullptr)
{
    if(_index > -1 && _index < _objList->size()) {
        _obj = _objList->at(_index);
    } else {

    }

}
template<typename T>
void AddObjCommand<T>::undo()
{
    if(_obj) {
        _objList->insert(_index,_obj);
    } else {
        _objList->pop_back();
    }
}
template<typename T>
void AddObjCommand<T>::redo()
{
    //means a remove
    if(_obj) {
        _objList->removeAt(_index);
    } else {
        _objList->append(Tucuxi::Gui::Core::CoreFactory::createEntity<T>(ABSTRACTREPO,0));
    }
}

template class AddObjCommand<Tucuxi::Gui::Core::ValidDose>;
template class AddObjCommand<Tucuxi::Gui::Core::ValidInterval>;
template class AddObjCommand<Tucuxi::Gui::Core::ValidInfusion>;

RemoveObjCommand::RemoveObjCommand(QObject *objList, int index, QUndoCommand *parent) :
    QUndoCommand(parent),
    _objList(objList),
    _index(index),
    _obj(nullptr)
{

     CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"size",Qt::DirectConnection,
                              Q_RETURN_ARG(int, _size)));

    if(_index > -1 && _index < _size) {
        QObject *retour;
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"objat",Qt::DirectConnection,
                                     Q_RETURN_ARG(QObject*, retour),
                                     Q_ARG(int, _index)));
        _obj = retour;
    } else {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"add",Qt::DirectConnection));



        QObject *retour;
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"objat",Qt::DirectConnection,
                                           Q_RETURN_ARG(QObject*,retour),
                                           Q_ARG(int,_size)));
        _obj = retour;
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"remove",Qt::DirectConnection,
                                           Q_ARG(int, _size)));
    }

}
void RemoveObjCommand::undo()
{

    if(_index > -1 && _index < _size) {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"objinsert",Qt::DirectConnection,
                                           Q_ARG(int, _index),
                                           Q_ARG(QObject*, _obj)));
    } else {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"remove",Qt::DirectConnection,
                                           Q_ARG(int, _size)));

    }
}

void RemoveObjCommand::redo()
{
    //means a remove
    if(_index > -1 && _index < _size) {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"remove",Qt::DirectConnection,
                                           Q_ARG(int, _index)));

//        _objList->removeAt(_index);
    } else {
        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"objappend",Qt::DirectConnection,
                                           Q_ARG(QObject*, _obj)));
//        CHECK_INVOKEMETHOD(QMetaObject::invokeMethod(_objList,"add",Qt::DirectConnection));
//        _objList->append(Tucuxi::Gui::Core::CoreFactory::createEntity<QObject>(ABSTRACTREPO,0));
    }
}
