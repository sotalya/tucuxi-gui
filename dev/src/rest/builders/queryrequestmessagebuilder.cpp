
#include <QDomDocument>

#include "queryrequestmessagebuilder.h"
#include "rest/model/queryrequestxmlmessage.h"


QueryRequestMessageBuilder::QueryRequestMessageBuilder(QObject *parent) :
    XmlMessageBuilder(parent)
{

}

QueryRequestMessageBuilder::~QueryRequestMessageBuilder()
{

}

Message *QueryRequestMessageBuilder::message() const
{
    QDomDocument doc;
    QDomElement rootNode = addDocumentElement(doc, "query_request");

    addNode(doc, rootNode, "request.id", "requestId");
    addNode(doc, rootNode, "patient.id", "patientId");
    addNode(doc, rootNode, "drug.id", "drugId");

    Message *message = new QueryRequestXmlMessage;
    message->setData(doc.toByteArray(2));

    Q_ASSERT(message->isValid());
    return message;
}

MessageContent QueryRequestMessageBuilder::messageContent() const
{
    Q_UNIMPLEMENTED();
    return MessageContent();
}
