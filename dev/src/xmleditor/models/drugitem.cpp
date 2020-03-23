#include "drugitem.h"
#include "drugparser.h"
#include "drugheadparser.h"

//Constructor
DrugItem::DrugItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns the drug name
QString DrugItem::name() const
{
    return DrugHeadParser(DrugParser(node()).headNode()).translatedName();
}

//Returns a drug parser class name
QByteArray DrugItem::parserType() const
{
    return DrugParser::staticMetaObject.className();
}
