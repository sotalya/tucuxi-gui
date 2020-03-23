#include "domitem.h"
#include "domitemfactory.h"

//Constructor
DomItem::DomItem(QDomElement node, DomItem *parent) : _node(node), _parent(parent), _initialized(false)
{

}

//Destructor
DomItem::~DomItem()
{
    foreach (DomItem *child, _children)
        delete child;
}

//Returns the display name
QString DomItem::name() const
{
    return Tag::translate(Tag::drugTag(node().tagName()));
}

//Returns the item node
QDomElement DomItem::node() const
{
    return _node;
}

//Returns the item row
int DomItem::row()
{
    //Check if the item isn't top-level
    if (_parent)
        return _parent->childRow(this);

    //If it is, return 0
    return 0;
}

//Returns a child's row
int DomItem::childRow(DomItem *child) const
{
    return _children.indexOf(child);
}

//Returns the number of children
int DomItem::childCount()
{
    //Check if the children are initialized
    if (!_initialized)
        initChildren();

    return _children.size();
}

//Returns the parent item
DomItem *DomItem::parent() const
{
    return _parent;
}

//Returns a child
DomItem *DomItem::child(int row)
{
    //Check if the children are initialized
    if (!_initialized)
        initChildren();

    //Check if the row is a valid index
    if (row < 0 || row >= _children.size())
        return 0;

    return _children[row];
}

//Removes a child
bool DomItem::removeChild(int row)
{
    //Check if the row is a valid index
    if (row < 0 || row >= _children.size())
        return false;

    //Retrieve the child
    DomItem *child = _children[row];

    //Remove the child
    _children.removeAt(row);

    //Delete the child
    delete child;

    return true;
}

//Inserts a child
bool DomItem::insertChild(QDomElement childNode, int row)
{
    //Check if the row is a valid index
    if (row < 0 || row > _children.size())
        return false;

    //Create the corresponding item
    DomItem *child = DomItemFactory().build(childNode, this);

    //Insert it at the given index
    _children.insert(row, child);

    return true;
}

//Appends a child
bool DomItem::appendChild(QDomElement childNode)
{
    //Create the corresponding item
    DomItem *child = DomItemFactory().build(childNode, this);

    //Insert it at the given index
    _children.append(child);

    return true;
}

//Prepends a child
bool DomItem::prependChild(QDomElement childNode)
{
    //Create the corresponding item
    DomItem *child = DomItemFactory().build(childNode, this);

    //Insert it at the given index
    _children.prepend(child);

    return true;
}

//Initializes the item children
void DomItem::initChildren()
{
    //Polymorphic call to initChildren()
    initChildrenImpl();

    //Set the initializer to true
    _initialized = true;
}

