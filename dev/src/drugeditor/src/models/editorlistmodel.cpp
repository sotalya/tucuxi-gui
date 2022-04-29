//@@license@@

#include "editorlistmodel.h"

EditorListModel::EditorListModel(QObject *parent):
    AbstractEntityListModel(parent),
    _editors(0),
    _roleNames()
{
    init();
}


EditorListModel::EditorListModel(QUndoStack *undoStack,QObject *parent):
    AbstractEntityListModel(undoStack,parent),
    _editors(0),
    _roleNames()
{
    init();
}


EditorListModel::~EditorListModel()
{

}


Qt::ItemFlags EditorListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return AbstractEntityListModel::flags(index) | Qt::ItemIsEditable;

}


QHash<int, QByteArray> EditorListModel::roleNames() const
{
    return _roleNames;
}


QVariant EditorListModel::data(const QModelIndex &index, int role) const
{

    if((!index.isValid()) || (index.row() >= rowCount()) )
        return QVariant();

    switch(role) {
    case NameRole :
        return _editors->at(index.row())->getName();
    case InstitutionRole:
        return _editors->at(index.row())->getInstitution();
    case EmailRole:
        return _editors->at(index.row())->getEmail();
    case DateRole:
        return _editors->at(index.row())->getDate();
    case CommentListRole:
        return (QVariant::fromValue(_editors->at(index.row())->getComments()));
        break;
    }


//    Q_ASSERT(false); //YMI: we get to this assert when calling `itemData()`
    return QVariant();
}

int EditorListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(_editors == nullptr)
        return 0;
    return _editors->count();
}


void EditorListModel::setModelData(QList<ezechiel::core::Editor*> *editors)
{
    beginResetModel();
    _editors = editors;
    endResetModel();
}


void EditorListModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    switch(role) {
    case NameRole :
        _editors->at(index.row())->setName(value.toString());
        break;
    case InstitutionRole:
        _editors->at(index.row())->setInstitution(value.toString());
        break;
    case EmailRole:
        _editors->at(index.row())->setEmail(value.toString());
        break;
    case DateRole:
        _editors->at(index.row())->setDate(value.toDateTime());
        break;
    case CommentListRole:
        _editors->at(index.row())->setComments(value.value<ezechiel::core::TranslatableString*>());
        break;
    default:
        Q_ASSERT(false);
        break;
    }

}

void EditorListModel::insertRowsImpl(int at, int count, const ezechiel::core::SharedEntitySet &entities)
{
    Q_ASSERT(at >= 0 && at <= rowCount());
    Q_ASSERT(count > 0);
    Q_ASSERT(entities.isEmpty() || entities.count() == count);

    ezechiel::core::Editor *editor;
    for (size_t i = 0; i<count; i++) {

        if(entities.isEmpty()){
            editor = ezechiel::core::CoreFactory::createEntity<ezechiel::core::Editor>(0);
            editor->setName("");
            editor->setEmail("");
            editor->setInstitution("");
            editor->setComments(ezechiel::core::CoreFactory::createEntity<ezechiel::core::TranslatableString>(0));
        } else {
            editor = static_cast<ezechiel::core::Editor*>(entities.at(i));
        }

        _editors->insert(i+at,editor);

    }

}

ezechiel::core::SharedEntitySet EditorListModel::removeRowsImpl(int at, int count)
{
    Q_ASSERT(at >= 0 && at + count <= rowCount());
    Q_ASSERT(count > 0);


    ezechiel::core::SharedEntitySet removed;
    for (int i = 0; i < count; ++i)
            removed << _editors->takeAt(at);

        return removed;

}


void EditorListModel::init()
{
    _roleNames[NameRole] = "name";
    _roleNames[InstitutionRole] = "institution";
    _roleNames[EmailRole] = "email";
    _roleNames[DateRole] = "date";
    _roleNames[CommentListRole] = "commentList";
}
