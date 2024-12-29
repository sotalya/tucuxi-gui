/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
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


#ifndef XMLMESSAGEBUILDER_H
#define XMLMESSAGEBUILDER_H



#include "messagebuilder.h"

#include <QString>
#include <QDomElement>

class QDomDocument;

namespace Tucuxi {
namespace Gui {
namespace Rest {




class XmlMessageBuilder : public MessageBuilder
{
    Q_OBJECT

public:
    explicit XmlMessageBuilder(QObject *parent = nullptr);
    virtual ~XmlMessageBuilder() Q_DECL_OVERRIDE;

protected:
    virtual QDomElement addDocumentElement(QDomDocument &doc, const QString &messageType, const QString &errorCondition = QString()) const;
    virtual void addNode(QDomDocument &doc, QDomElement &parent, const QString &key, const QString &tag, bool required = true) const;
};

}
}
}

#endif // XMLMESSAGEBUILDER_H
