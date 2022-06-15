//@@license@@
#include "acknotificationxmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


const char *const AckNotificationXmlMessage::_MESSAGE_TYPE = "ACK_NOTIFICATION";

AckNotificationXmlMessage::AckNotificationXmlMessage(QObject *parent) :
    IngoingXmlMessage(parent)
{

}

AckNotificationXmlMessage::~AckNotificationXmlMessage()
{

}

QString AckNotificationXmlMessage::schemaPath() const
{
    return Tucuxi::Gui::Core::XmlValidator::Acks;
}

QString AckNotificationXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}

}
}
}
