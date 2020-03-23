#include "parameteritem.h"
#include "parameterparser.h"

//Constructor
ParameterItem::ParameterItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the name of the parameter
QString ParameterItem::name() const
{
    return ParameterParser(node()).parameterId();
}

//Returns a parameter parser class name
QByteArray ParameterItem::parserType() const
{
    return ParameterParser::staticMetaObject.className();
}
