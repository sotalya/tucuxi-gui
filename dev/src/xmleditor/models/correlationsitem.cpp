#include "correlationsitem.h"
#include "correlationsparser.h"

//Constructor
CorrelationsItem::CorrelationsItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a correlation parser class name
QByteArray CorrelationsItem::parserType() const
{
    return CorrelationsParser::staticMetaObject.className();
}
