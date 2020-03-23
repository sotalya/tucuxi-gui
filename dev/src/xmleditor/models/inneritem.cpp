#include "inneritem.h"

//Constructor
InnerItem::InnerItem(QDomElement node, DomItem *parent) : DomItem(node, parent)
{

}

//Destructor
InnerItem::~InnerItem()
{

}

//Initializes the item children
void InnerItem::initChildrenImpl()
{
    //Get the node first child
    QDomElement child = node().firstChildElement();

    //For each of the node child
    while (!child.isNull()) {

        //Construct and add child
        appendChild(child);

        //Move to the next child
        child = child.nextSiblingElement();
    }
}

