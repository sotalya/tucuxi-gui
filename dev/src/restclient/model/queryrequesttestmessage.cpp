#include "queryrequesttestmessage.h"
#include "restclient/utils/xmltestlibrary.h"

QueryRequestTestMessage::QueryRequestTestMessage(const QString &forRequestId, QObject *parent) :
    QueryRequestXmlMessage(parent),
    _requestId(forRequestId)
{

}

QueryRequestTestMessage::~QueryRequestTestMessage()
{

}

QByteArray QueryRequestTestMessage::data() const
{
    return XmlTestLibrary::getInstance().queryRequest(_requestId).toByteArray();
}
