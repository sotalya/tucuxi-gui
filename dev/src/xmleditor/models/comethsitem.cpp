#include "comethsitem.h"
#include "comethsparser.h"

//Constructor
ComethsItem::ComethsItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a comeths parser class name
QByteArray ComethsItem::parserType() const
{
    return ComethsParser::staticMetaObject.className();
}
