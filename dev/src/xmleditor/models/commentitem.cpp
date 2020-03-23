#include "commentitem.h"
#include "commentparser.h"

//Constructor
CommentItem::CommentItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns a comment parser class name
QByteArray CommentItem::parserType() const
{
    return CommentParser::staticMetaObject.className();
}
