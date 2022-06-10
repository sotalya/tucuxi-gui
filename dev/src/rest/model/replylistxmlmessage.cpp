//@@license@@

#include "replylistxmlmessage.h"
#include "core/utils/xmlvalidator.h"

const char *const ReplyListXmlMessage::_MESSAGE_TYPE = "REPLY_LIST";

ReplyListXmlMessage::ReplyListXmlMessage(QObject *parent) :
    IngoingXmlMessage(parent)
{

}

ReplyListXmlMessage::~ReplyListXmlMessage()
{

}

QString ReplyListXmlMessage::schemaPath() const
{
    return Tucuxi::GuiCore::XmlValidator::Reply_List;
}

QString ReplyListXmlMessage::messageType() const
{
    return _MESSAGE_TYPE;
}
