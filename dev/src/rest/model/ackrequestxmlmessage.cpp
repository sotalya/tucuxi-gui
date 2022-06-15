//@@license@@

#include "ackrequestxmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const AckRequestXmlMessage::_MESSAGE_TYPE = "ACK_REQUEST";

AckRequestXmlMessage::AckRequestXmlMessage(QObject *parent) :
    OutgoingXmlMessage(parent)
{

}

AckRequestXmlMessage::~AckRequestXmlMessage()
{

}

QString AckRequestXmlMessage::schemaPath() const
{
    return Tucuxi::Gui::Core::XmlValidator::Acks;
}

QString AckRequestXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}

}
}
}
