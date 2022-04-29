//@@license@@

#include "notificationtestmessage.h"
#include "restclient/utils/xmltestlibrary.h"

NotificationTestMessage::NotificationTestMessage(const QString &forRequestId, QObject *parent) :
    NotificationXmlMessage(parent),
    _requestId(forRequestId)
{

}

NotificationTestMessage::~NotificationTestMessage()
{

}

QByteArray NotificationTestMessage::data() const
{
    return XmlTestLibrary::getInstance().notification(_requestId).toByteArray();
}
