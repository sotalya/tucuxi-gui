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


#include "xmlmessage.h"
#include "core/utils/xmlvalidator.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


XmlMessage::XmlMessage(QObject *parent) :
    Message(parent),
    _data()
{

}

void XmlMessage::setData(QByteArray data)
{
    _data = data;
}

QByteArray XmlMessage::data() const
{
    return _data;
}

XmlMessage::~XmlMessage()
{

}

bool XmlMessage::isValid()
{
    QByteArray d = data();
    if (d.isEmpty()) {
        setErrorString(tr("The message type '%1' appears to be empty").arg(messageType()));
        return false;
    }

    Tucuxi::Gui::Core::XmlValidator validator;
    if (!validator.validate(d, schemaPath())) {
        setErrorString(tr("The message type '%1' is invalid (%2)").arg(messageType(), validator.errorMessage()));
        return false;
    }

    return true;
}

}
}
}
