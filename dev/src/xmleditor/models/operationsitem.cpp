#include "operationsitem.h"
#include "operationsparser.h"

//Constructor
OperationsItem::OperationsItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns an operation parser class name
QByteArray OperationsItem::parserType() const
{
    return OperationsParser::staticMetaObject.className();
}
