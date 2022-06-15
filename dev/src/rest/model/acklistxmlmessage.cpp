//@@license@@

#include "acklistxmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


const char *const AckListXmlMessage::_MESSAGE_TYPE = "ACK_LIST";

AckListXmlMessage::AckListXmlMessage(QObject *parent) :
    OutgoingXmlMessage(parent)
{

}

AckListXmlMessage::~AckListXmlMessage()
{

}

QString AckListXmlMessage::schemaPath() const
{
    return Tucuxi::Gui::Core::XmlValidator::Acks;
}

QString AckListXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}

}
}
}
