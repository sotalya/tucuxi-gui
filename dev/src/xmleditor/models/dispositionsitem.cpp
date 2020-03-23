#include "dispositionsitem.h"
#include "dispositionsparser.h"

//Constructor
DispositionsItem::DispositionsItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a dispositions parser class name
QByteArray DispositionsItem::parserType() const
{
    return DispositionsParser::staticMetaObject.className();
}
