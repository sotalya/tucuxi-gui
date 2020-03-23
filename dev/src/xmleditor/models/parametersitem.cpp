#include "parametersitem.h"
#include "parametersparser.h"

//Constructor
ParametersItem::ParametersItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a parameters parser class name
QByteArray ParametersItem::parserType() const
{
    return ParametersParser::staticMetaObject.className();
}
