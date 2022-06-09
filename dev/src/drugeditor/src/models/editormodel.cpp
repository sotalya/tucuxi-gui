//@@license@@

#include "editormodel.h"



EditorModel::EditorModel(QObject *parent):
    AbstractEntityModel(parent),
    _editor(),
    _roleNames()
{
    init();
}


EditorModel::EditorModel(QUndoStack *undoStack,QObject *parent):
    AbstractEntityModel(undoStack,parent),
    _editor(),
    _roleNames()
{
    init();
}


EditorModel::~EditorModel()
{

}


Qt::ItemFlags EditorModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::ItemIsEnabled;
    }

    return AbstractEntityModel::flags(index) | Qt::ItemIsEditable;

}


QHash<int, QByteArray> EditorModel::roleNames() const
{
    return _roleNames;
}


QVariant EditorModel::data(const QModelIndex &index, int role) const
{

    if((!index.isValid()) || (index.row() >= rowCount()) )
        return QVariant();

    switch(role) {
    case NameRole :
        return (_editor)->getName();
    case InstitutionRole:
        return (_editor)->getInstitution();
    case EmailRole:
        return (_editor)->getEmail();
    case DateRole:
        return (_editor)->getDate();
    case CommentKeysRole:
        return (QVariant::fromValue(QStringList((_editor)->getComments()->keys())));
        break;
    case CommentValuesRole:
        return (QVariant::fromValue(QStringList((_editor)->getComments()->values())));
        break;
    }


    Q_ASSERT(false);
    return QVariant();
}

int EditorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _editor ? 1 : 0;
}


void EditorModel::setModelData(ezechiel::GuiCore::Editor *editor)
{
    beginResetModel();
    _editor = editor;
    endResetModel();
}
ezechiel::GuiCore::Editor* EditorModel::getModelData()
{
    return _editor;
}


void EditorModel::setDataImpl(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < rowCount());
    Q_ASSERT(value.isValid());
    Q_ASSERT(_roleNames.contains(role));

    switch(role) {
    case NameRole :
        (_editor)->setName(value.toString());
        break;
    case InstitutionRole:
        (_editor)->setInstitution(value.toString());
        break;
    case EmailRole:
        (_editor)->setEmail(value.toString());
        break;
    case DateRole:
        (_editor)->setDate(value.toDateTime());
        break;
//    case CommentListRole:
//        (_editor)->setCommentsTranslationMap(value.value<ezechiel::GuiCore::QStringTranslation>());
//        break;
    default:
        Q_ASSERT(false);
        break;
    }

}


void EditorModel::init()
{
    _roleNames[NameRole] = "name";
    _roleNames[InstitutionRole] = "institution";
    _roleNames[EmailRole] = "email";
    _roleNames[DateRole] = "date";
    _roleNames[CommentKeysRole] = "commentKeys";
    _roleNames[CommentValuesRole] = "commentValues";
}
