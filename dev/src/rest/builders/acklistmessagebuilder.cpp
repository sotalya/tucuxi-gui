//@@license@@

#include "acklistmessagebuilder.h"
#include "rest/model/acklistxmlmessage.h"

#include <QDomDocument>

AckListMessageBuilder::AckListMessageBuilder(QObject *parent) :
    XmlMessageBuilder(parent)
{

}

AckListMessageBuilder::~AckListMessageBuilder()
{

}

Message *AckListMessageBuilder::message() const
{
    QDomDocument doc;
    QDomElement rootNode = addDocumentElement(doc, "ack_list");

    addNode(doc, rootNode, "replyControlId", "replyControlId");
    addNode(doc, rootNode, "ackCode", "acknowledgmentCode");

    if (_content.isValueSet("ackMessage"))
        addNode(doc, rootNode, "ackMessage", "acknowledgmentMessage");

    Message *message = new AckListXmlMessage;
    message->setData(doc.toByteArray(2));

    Q_ASSERT(message->isValid());
    return message;
}

MessageContent AckListMessageBuilder::messageContent() const
{
    Q_UNIMPLEMENTED();
    return MessageContent();
}
