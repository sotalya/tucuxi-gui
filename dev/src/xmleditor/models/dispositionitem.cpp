#include "dispositionitem.h"
#include "dispositionparser.h"
#include "dispositionheadparser.h"

//Constructor
DispositionItem::DispositionItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns the disposition name
QString DispositionItem::name() const
{
    //Get a parser for the head
    DispositionHeadParser head(DispositionParser(node()).headNode());

    //Get and return the name
    QString name = head.translatedName();

    if (!name.isEmpty())
        return name;

    //If it's empty, return the ID
    return head.id();
}

//Returns a disposition parser class name
QByteArray DispositionItem::parserType() const
{
    return DispositionParser::staticMetaObject.className();
}
