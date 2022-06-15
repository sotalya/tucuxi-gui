//@@license@@

#include "queryrequestxmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

const char *const QueryRequestXmlMessage::_MESSAGE_TYPE = "QUERY_REQUEST";

QueryRequestXmlMessage::QueryRequestXmlMessage(QObject *parent) :
    OutgoingXmlMessage(parent)
{

}

QueryRequestXmlMessage::~QueryRequestXmlMessage()
{

}

QString QueryRequestXmlMessage::schemaPath() const
{
    return Tucuxi::Gui::Core::XmlValidator::Queries;
}

QString QueryRequestXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}

}
}
}
