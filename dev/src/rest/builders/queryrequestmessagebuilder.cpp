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



#include <QDomDocument>

#include "queryrequestmessagebuilder.h"
#include "rest/model/queryrequestxmlmessage.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {

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

}
}
}
