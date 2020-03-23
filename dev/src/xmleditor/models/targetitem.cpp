#include "targetitem.h"
#include "targetparser.h"
#include "core.h"
#include "definitionmanager.h"

//Constructor
TargetItem::TargetItem(QDomElement node, DomItem *parent) : OuterItem(node, parent)
{

}

//Returns the target name
QString TargetItem::name() const
{
    //Get and return the target type name
    QString name = CORE->definitionManager()->target(TargetParser(node()).targetType()).name;

    if (!name.isEmpty())
        return name;

    //If it's empty, return the tag default name
    return OuterItem::name();
}

//Returns a target parser class name
QByteArray TargetItem::parserType() const
{
    return TargetParser::staticMetaObject.className();
}
