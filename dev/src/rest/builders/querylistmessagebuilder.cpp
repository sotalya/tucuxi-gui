#include <QDomDocument>

#include "querylistmessagebuilder.h"
#include "rest/model/querylistxmlmessage.h"


QueryListMessageBuilder::QueryListMessageBuilder(QObject *parent) :
    XmlMessageBuilder(parent)
{
}

QueryListMessageBuilder::~QueryListMessageBuilder()
{
}

Message *QueryListMessageBuilder::message() const
{
    QDomDocument doc;
    QDomElement rootNode = addDocumentElement(doc, "query_list");

    addNode(doc, rootNode, "from", "from");
    addNode(doc, rootNode, "to", "to");
    addNode(doc, rootNode, "state", "state");

    Message *message = new QueryListXmlMessage;
    message->setData(doc.toByteArray(2));

    Q_ASSERT(message->isValid());
    return message;
}

MessageContent QueryListMessageBuilder::messageContent() const
{
    Q_UNIMPLEMENTED();
    return MessageContent();
}
