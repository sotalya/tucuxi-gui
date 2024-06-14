/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "xmlmessagebuilder.h"

#include "rest/utils/controlid.h"

#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace Rest {


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

}
}
}
