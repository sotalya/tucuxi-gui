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


#include "acklistmessagebuilder.h"
#include "rest/model/acklistxmlmessage.h"

#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace Rest {


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

}
}
}
