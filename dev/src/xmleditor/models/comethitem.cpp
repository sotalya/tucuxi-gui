#include "comethitem.h"
#include "core.h"
#include "pluginmanager.h"
#include "comethparser.h"
#include "comethheadparser.h"

//Default contructor
ComethItem::ComethItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns the cometh name
QString ComethItem::name() const
{
    //Get a parser for the head
    ComethHeadParser head(ComethParser(node()).headNode());

    //Get and return the plugin name
    QString name = CORE->pluginManager()->model(head.comethId()).name;

    if (!name.isEmpty())
        return name;

    //If it's empty, return the ID
    return head.id();
}

//Returns a cometh parser class name
QByteArray ComethItem::parserType() const
{
    return ComethParser::staticMetaObject.className();
}
