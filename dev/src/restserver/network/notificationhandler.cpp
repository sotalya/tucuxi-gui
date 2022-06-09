#include "notificationhandler.h"

#include "core/utils/xmlvalidator.h"

#include "rest/model/notificationxmlmessage.h"
#include "rest/utils/controlid.h"

#include <QString>
#include <QVariant>

NotificationHandler::NotificationHandler(QObject *parent) :
    HttpRequestHandler(parent),
    ErrorHandler()
{

}

NotificationHandler::~NotificationHandler()
{

}

void NotificationHandler::service(HttpRequest &request, HttpResponse &response)
{
    NotificationXmlMessage message;
    message.setData(request.getBody());
    if (!message.isValid()) {
        badRequest(response, "Malformed request");
        return;
    }

    QDomDocument notification;
    if (!notification.setContent(request.getBody())) {
        internalError(response, "Unkown convertion error");
        return;
    }

    ezechiel::GuiCore::XmlValidator validator;
    QDomDocument ack_notifiation = buildAck(notification);
    if (!validator.validate(ack_notifiation.toByteArray(), ezechiel::GuiCore::XmlValidator::Acks)) {
        internalError(response,QString("Malformed response (%1)").arg(validator.errorMessage()));
        return;
    }

    response.setHeader("Content-Type", "text/xml");
    response.write(ack_notifiation.toByteArray(2), true);
}

QString NotificationHandler::handlerName() const
{
    return metaObject()->className();
}

QDomDocument NotificationHandler::buildAck(const QDomDocument &source) const
{
    QDomDocument result;

    //The <data> node
    QDomElement rootNode = result.createElement("data");
    result.appendChild(rootNode);

    QDomAttr versionAttr = result.createAttribute("version");
    versionAttr.setNodeValue("0.2");
    rootNode.setAttributeNode(versionAttr);

    QDomAttr langAttr = result.createAttribute("lang");
    langAttr.setNodeValue("en");
    rootNode.setAttributeNode(langAttr);

    QDomAttr typeAttr = result.createAttribute("type");
    typeAttr.setNodeValue("ack_notification");
    rootNode.setAttributeNode(typeAttr);

    QDomAttr controlIdAttr = result.createAttribute("controlId");
    controlIdAttr.setNodeValue(QString::number(ControlId::getInstance().next()));
    rootNode.setAttributeNode(controlIdAttr);

    QDomAttr xsiAttr = result.createAttribute("xmlns:xsi");
    xsiAttr.setNodeValue("http://www.w3.org/2001/XMLSchema-instance");
    rootNode.setAttributeNode(xsiAttr);

    QDomAttr nnslAttr = result.createAttribute("xsi:noNamespaceSchemaLocation");
    nnslAttr.setNodeValue("eep.xsd");
    rootNode.setAttributeNode(nnslAttr);

    //The <replyControlId> node
    QDomElement replyControlIdNode = result.createElement("replyControlId");
    rootNode.appendChild(replyControlIdNode);

    QDomText replyControlIdText = result.createTextNode(source.documentElement().attributeNode("controlId").value());
    replyControlIdNode.appendChild(replyControlIdText);

    //The <acknowledgmentCode> node
    QDomElement acknowledgmentCodeNode = result.createElement("acknowledgmentCode");
    rootNode.appendChild(acknowledgmentCodeNode);

    QDomText acknowledgmentCodeText = result.createTextNode("AA");
    acknowledgmentCodeNode.appendChild(acknowledgmentCodeText);

    return result;
}
