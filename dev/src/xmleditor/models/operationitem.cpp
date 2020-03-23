#include "operationitem.h"
#include "operationparser.h"

//Constructor
OperationItem::OperationItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the name of the operation parameter
QString OperationItem::name() const
{
    //Get, verify and return the operation parameter ID
    QString name = OperationParser(node()).parameter();

    if (!name.isEmpty())
        return name;

    //If it's empty, return the default tag name
    return OuterItem::name();
}

//Returns an operation parser class name
QByteArray OperationItem::parserType() const
{
    return OperationParser::staticMetaObject.className();
}
