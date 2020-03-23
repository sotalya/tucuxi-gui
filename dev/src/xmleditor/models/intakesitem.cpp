#include "intakesitem.h"
#include "intakesparser.h"

//Constructor
IntakesItem::IntakesItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns an intakes parser class name
QByteArray IntakesItem::parserType() const
{
    return IntakesParser::staticMetaObject.className();
}
