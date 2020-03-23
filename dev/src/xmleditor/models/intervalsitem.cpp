#include "intervalsitem.h"
#include "intervalsparser.h"

//Constructor
IntervalsItem::IntervalsItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns an intervals parser class name
QByteArray IntervalsItem::parserType() const
{
    return IntervalsParser::staticMetaObject.className();
}
