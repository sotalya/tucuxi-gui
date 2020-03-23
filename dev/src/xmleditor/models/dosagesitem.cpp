#include "dosagesitem.h"
#include "dosagesparser.h"

//Constructor
DosagesItem::DosagesItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a dosages parser class name
QByteArray DosagesItem::parserType() const
{
    return DosagesParser::staticMetaObject.className();
}
