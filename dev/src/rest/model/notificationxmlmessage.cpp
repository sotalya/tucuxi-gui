#include "notificationxmlmessage.h"
#include "core/utils/xmlvalidator.h"

const char *const NotificationXmlMessage::_MESSAGE_TYPE = "NOTIFICATION";

NotificationXmlMessage::NotificationXmlMessage(QObject *parent) :
    OutgoingXmlMessage(parent)
{

}

NotificationXmlMessage::~NotificationXmlMessage()
{

}

QString NotificationXmlMessage::schemaPath() const
{
    return ezechiel::core::XmlValidator::Notification;
}

QString NotificationXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}
