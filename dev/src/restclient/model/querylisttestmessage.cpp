#include "querylisttestmessage.h"
#include "restclient/utils/xmltestlibrary.h"

QueryListTestMessage::QueryListTestMessage(QObject *parent) :
    QueryListXmlMessage(parent)
{

}

QueryListTestMessage::~QueryListTestMessage()
{

}

QByteArray QueryListTestMessage::data() const
{
    return XmlTestLibrary::getInstance().queryList().toByteArray();
}
