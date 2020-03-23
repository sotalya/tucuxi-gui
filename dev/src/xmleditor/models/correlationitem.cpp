#include "correlationitem.h"
#include "correlationparser.h"

//Constructor
CorrelationItem::CorrelationItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the correlation name
QString CorrelationItem::name() const
{
    //Get, build and return the correlation name
    CorrelationParser correlation(node());

    if (correlation.isValid())
        return correlation.param1Id() + "/" + correlation.param2Id();

    //Otherwise return the default tag name
    return OuterItem::name();
}

//Returns a correlation parser class name
QByteArray CorrelationItem::parserType() const
{
    return CorrelationParser::staticMetaObject.className();
}
