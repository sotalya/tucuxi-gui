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

#include "querylistmessagebuilder.h"
#include "rest/model/querylistxmlmessage.h"


namespace Tucuxi {
namespace Gui {
namespace Rest {


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

}
}
}
