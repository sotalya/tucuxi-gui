//@@license@@

#include "xmlmessagebuilder.h"

#include "rest/utils/controlid.h"

#include <QDomDocument>

XmlMessageBuilder::XmlMessageBuilder(QObject *parent) :
    MessageBuilder(parent)
{

}

XmlMessageBuilder::~XmlMessageBuilder()
{

}

QDomElement XmlMessageBuilder::addDocumentElement(QDomDocument &doc, const QString &messageType, const QString &errorCondition) const
{
    QDomElement rootNode = doc.createElement("data");
    doc.appendChild(rootNode);

    QDomAttr versionAttr = doc.createAttribute("version");
    versionAttr.setNodeValue("0.2");
    rootNode.setAttributeNode(versionAttr);

    QDomAttr langAttr = doc.createAttribute("lang");
    langAttr.setNodeValue("en");
    rootNode.setAttributeNode(langAttr);

    QDomAttr typeAttr = doc.createAttribute("type");
    typeAttr.setNodeValue(messageType);
    rootNode.setAttributeNode(typeAttr);

    QDomAttr controlIdAttr = doc.createAttribute("controlId");
    controlIdAttr.setNodeValue(QString::number(ControlId::getInstance().next()));
    rootNode.setAttributeNode(controlIdAttr);

    if (!errorCondition.isEmpty()) {
        QDomAttr errorConditionAttr = doc.createAttribute("errorCondition");
        errorConditionAttr.setNodeValue(errorCondition);
        rootNode.setAttributeNode(errorConditionAttr);
    }

    QDomAttr xsiAttr = doc.createAttribute("xmlns:xsi");
    xsiAttr.setNodeValue("http://www.w3.org/2001/XMLSchema-instance");
    rootNode.setAttributeNode(xsiAttr);

    QDomAttr nnslAttr = doc.createAttribute("xsi:noNamespaceSchemaLocation");
    nnslAttr.setNodeValue("eep.xsd");
    rootNode.setAttributeNode(nnslAttr);

    return rootNode;
}

void XmlMessageBuilder::addNode(QDomDocument &doc, QDomElement &parent, const QString &key, const QString &tag, bool required) const
{
    Q_ASSERT(!required || _content.isValueSet(key));

    QDomElement node = doc.createElement(tag);
    parent.appendChild(node);

    QDomText text = doc.createTextNode(_content.value(key));
    node.appendChild(text);
}
