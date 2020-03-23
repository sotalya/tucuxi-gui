#include "targetsitem.h"
#include "targetsparser.h"

//Constructor
TargetsItem::TargetsItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a targets parser class name
QByteArray TargetsItem::parserType() const
{
    return TargetsParser::staticMetaObject.className();
}
