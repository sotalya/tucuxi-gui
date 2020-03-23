#include "revisionitem.h"
#include "authorparser.h"

//Constructor
RevisionItem::RevisionItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the revision name
QString RevisionItem::name() const
{
    return RevisionParser(node()).date().toString(Qt::SystemLocaleShortDate);
}

//Returns a revision parser class name
QByteArray RevisionItem::parserType() const
{
    return RevisionParser::staticMetaObject.className();
}
