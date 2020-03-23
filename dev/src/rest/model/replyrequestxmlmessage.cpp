#include "replyrequestxmlmessage.h"
#include "core/utils/xmlvalidator.h"

const char *const ReplyRequestXmlMessage::_MESSAGE_TYPE = "REPLY_REQUEST";

ReplyRequestXmlMessage::ReplyRequestXmlMessage(QObject *parent) :
    IngoingXmlMessage(parent)
{

}

ReplyRequestXmlMessage::~ReplyRequestXmlMessage()
{

}

QString ReplyRequestXmlMessage::schemaPath() const
{
    return ezechiel::core::XmlValidator::Reply_Request;
}

QString ReplyRequestXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}
