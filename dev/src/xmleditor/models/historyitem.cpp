#include "historyitem.h"
#include "historyparser.h"

//Constructor
HistoryItem::HistoryItem(QDomElement node, DomItem *parent) : InnerItem(node, parent)
{

}

//Returns a history parser class name
QByteArray HistoryItem::parserType() const
{
    return HistoryParser::staticMetaObject.className();
}
