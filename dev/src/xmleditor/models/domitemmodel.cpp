#include "domitemmodel.h"

//Model's number of columns
const int DomItemModel::_nbColumns = 1;

//Default constructor
DomItemModel::DomItemModel(QObject *parent) : QAbstractItemModel(parent)
{

}

//Returns an item's flags
Qt::ItemFlags DomItemModel::flags(const QModelIndex &index) const
{
    //Check if the index is valid
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

//Returns an item's data
QVariant DomItemModel::data(const QModelIndex &index, int role) const
{
    //Check if the index is valid
    if (!index.isValid())
        return QVariant();

    //Check if the column is valid
    if (index.column() > _nbColumns - 1)
        return QVariant();

    //Get the corresponding item
    DomItem *item = static_cast<DomItem *>(index.internalPointer());

    //Return the display name
    if (role == Qt::DisplayRole)
        return item->name();

    return QVariant();
}

//Returns a header's data
QVariant DomItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Return empty data if the root is null
    if (_root.isNull())
        return QVariant();

    //Return empty data for vertical headers
    if (orientation != Qt::Horizontal)
        return QVariant();

    //Return empty data for invalid columns
    if (section > _nbColumns - 1)
        return QVariant();

    //Return the root element name
    if (role == Qt::DisplayRole)
        return _root->name();

    return QVariant();
}

//Returns an item's index
QModelIndex DomItemModel::index(int row, int column, const QModelIndex &parent) const
{
    //Check if the index is in range
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    //Get the parent item
    DomItem *parentItem;

    if (!parent.isValid()) //Invalid parent refers to top-level item
        parentItem = _root.data();
    else
        parentItem = static_cast<DomItem *>(parent.internalPointer());

    //Get the child item
    DomItem *childItem = parentItem->child(row);

    //Check if the child is valid
    if (!childItem)
        return QModelIndex();

    //Create and return the child index
    return createIndex(row, column, childItem);
}

//Returns an item's parent index
QModelIndex DomItemModel::parent(const QModelIndex &child) const
{
    //Check if the index is valid
    if (!child.isValid())
        return QModelIndex();

    //Get the child item
    DomItem *childItem = static_cast<DomItem *>(child.internalPointer());

    //Get the parent item
    DomItem *parentItem = childItem->parent();

    //Check if the child is top-level
    if (!parentItem)
        return QModelIndex();

    //Create and return the parent index
    return createIndex(parentItem->row(), 0, parentItem);
}

//Returns an item's number of children
int DomItemModel::rowCount(const QModelIndex &parent) const
{
    //Check if the root exists
    if (_root.isNull())
        return 0;

    //Check if the column is valid
    if (parent.column() > _nbColumns - 1)
        return 0;

    //Get the parent item
    DomItem *parentItem;

    if (!parent.isValid()) //Invalid parent refers to top-level item
        parentItem = _root.data();
    else
        parentItem = static_cast<DomItem *>(parent.internalPointer());

    //Return the number of children
    return parentItem->childCount();
}

//Returns the model's number of columns
int DomItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _nbColumns;
}

//Initializes the model with a DOM document
void DomItemModel::setDocument(QDomDocument document)
{
    //Start reseting the model
    beginResetModel();

    //Set the model's document
    _document = document;

    //Replace the root item
    _root.reset(_factory.build(document.documentElement()));

    //Finish resetting the model
    endResetModel();
}

//Clears the model
void DomItemModel::clear()
{
    //Start resetting the model
    beginResetModel();

    //Clear the model's document
    _document = QDomDocument();

    //Clear the root DOM item
    _root.reset();

    //Finish resetting the model
    endResetModel();
}
