//@@license@@

#include "notificationxmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

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
    return Tucuxi::Gui::Core::XmlValidator::Notification;
}

QString NotificationXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}

}
}
}
