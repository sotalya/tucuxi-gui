#include "revisionsitem.h"
#include "revisionsparser.h"

//Constructor
RevisionsItem::RevisionsItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a revisions parser class name
QByteArray RevisionsItem::parserType() const
{
    return RevisionsParser::staticMetaObject.className();
}
