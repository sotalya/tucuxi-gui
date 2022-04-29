//@@license@@

#include "querylistxmlmessage.h"
#include "core/utils/xmlvalidator.h"

const char *const QueryListXmlMessage::_MESSAGE_TYPE = "QUERY_LIST";

QueryListXmlMessage::QueryListXmlMessage(QObject *parent) :
    OutgoingXmlMessage(parent)
{

}

QueryListXmlMessage::~QueryListXmlMessage()
{

}

QString QueryListXmlMessage::schemaPath() const
{
    return ezechiel::core::XmlValidator::Queries;
}

QString QueryListXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}
