#include "errormodelitem.h"
#include "errormodelparser.h"

//Constructor
ErrorModelItem::ErrorModelItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns an error model parser class name
QByteArray ErrorModelItem::parserType() const
{
    return ErrorModelParser::staticMetaObject.className();
}
