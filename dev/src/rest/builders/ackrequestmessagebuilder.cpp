//@@license@@

#include <QDomDocument>

#include "ackrequestmessagebuilder.h"
#include "rest/model/ackrequestxmlmessage.h"


AckRequestMessageBuilder::AckRequestMessageBuilder(QObject *parent) :
    XmlMessageBuilder(parent)
{

}

AckRequestMessageBuilder::~AckRequestMessageBuilder()
{

}

Message *AckRequestMessageBuilder::message() const
{
    QDomDocument doc;
    QDomElement rootNode = addDocumentElement(doc, "ack_request");

    addNode(doc, rootNode, "replyControlId", "replyControlId");
    addNode(doc, rootNode, "ackCode", "acknowledgmentCode");

    if (_content.isValueSet("ackMessage"))
        addNode(doc, rootNode, "ackMessage", "acknowledgmentMessage");

    Message *message = new AckRequestXmlMessage;
    message->setData(doc.toByteArray(2));

    Q_ASSERT(message->isValid());
    return message;
}

MessageContent AckRequestMessageBuilder::messageContent() const
{
    Q_UNIMPLEMENTED();
    return MessageContent();
}
