//@@license@@

#include "commentlistmodel.h"

CommentListModel::CommentListModel(QObject *parent):
    AbstractEntityListModel(parent),
    _comments(),
    _keys(),
    _roleNames()
{
    init();
}

CommentListModel::CommentListModel(QUndoStack *undoStack, QObject *parent):
    AbstractEntityListModel(undoStack,parent),
    _comments(),
    _keys(),
    _roleNames()
{
    init();
}


CommentListModel::~CommentListModel()
{

}

Qt::ItemFlags CommentListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> CommentListModel::roleNames() const
{
    return _roleNames;
}

QVariant CommentListModel::data(const QModelIndex &index, int role) const
{

    if((!index.isValid()) || (index.row() >= rowCount()) )
        return QVariant();
    switch(role){
    case LangRole:
        return _comments->keys().at(index.row());
    case CommentRole:
        return _comments->values().at(index.row());
    }

    return QVariant();
}

int CommentListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(_comments == nullptr)
        return 0;

    return _comments->size();
}

void CommentListModel::setModelData(ezechiel::GuiCore::TranslatableString *comments)
{
    beginResetModel();
    _comments = comments;

    keyGenAcc = 26;
    endResetModel();


}

//FIXME probably
void CommentListModel::setDataImpl(const QModelIndex &index, const QVariant &newValue, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(newValue.isValid());
    Q_ASSERT(_roleNames.contains(role));

    QString key = _comments->keys().at(index.row());

    switch (role) {
    case LangRole:
        _comments->insert(newValue.toString(),_comments->take(key));
        break;
    case CommentRole:
        _comments->remove(key);
        _comments->insert(key,newValue.toString());
        break;
    default:
        break;
    }
}

void CommentListModel::insertRowsImpl(int at, int count, const ezechiel::GuiCore::SharedEntitySet &entities){

    Q_ASSERT(at >= 0 && count >= 0);
    Q_ASSERT(at+count <= rowCount()+1);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);
    for (size_t i = 0; i < count; ++i) {
        QString key, value;

        if(entities.isEmpty()){
            key = keyGen();
            value = "";
        } else {
            ezechiel::GuiCore::TranslatableString *comment = static_cast<ezechiel::GuiCore::TranslatableString*>(entities.at(i));
            key = comment->keys().at(0);
            value = comment->value(key);
        }
        _comments->insert(key,value);
    }

}

ezechiel::GuiCore::SharedEntitySet CommentListModel::removeRowsImpl(int at, int count){

    Q_ASSERT(at >= 0 && count >= 0);
    Q_ASSERT(at+count <= rowCount());

    QStringList keys =_comments->keys();

    ezechiel::GuiCore::SharedEntitySet removed;
    for (int i = 0; i < count; ++i) {
        ezechiel::GuiCore::TranslatableString *trans = ezechiel::GuiCore::CoreFactory::createEntity<ezechiel::GuiCore::TranslatableString>(0);
        QString key = keys.at(i+at);
        trans->insert(key,_comments->take(key));
        removed << trans;
    }

    return removed;
}

void CommentListModel::init(){
    _roleNames[LangRole] = "lang";
    _roleNames[CommentRole] = "comment";
}


QString CommentListModel::keyGen()
{

    int acc = keyGenAcc++;
    QString retour = "";
    do{
        retour = char('A' + acc%26) +retour;
        acc /= 26;
    } while(acc > 0);

    return (_comments->contains(retour) ? keyGen() : retour);
}
