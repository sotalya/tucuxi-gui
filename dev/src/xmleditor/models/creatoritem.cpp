#include "creatoritem.h"
#include "authorparser.h"

//Constructor
CreatorItem::CreatorItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns a creator parser class name
QByteArray CreatorItem::parserType() const
{
    return CreatorParser::staticMetaObject.className();
}
