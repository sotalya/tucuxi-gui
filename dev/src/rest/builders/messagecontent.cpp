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


#include "messagecontent.h"

namespace Tucuxi {
namespace Gui {
namespace Rest {


MessageContent::MessageContent() :
    content(),
    lists()
{

}

MessageContent::~MessageContent()
{

}

QString MessageContent::value(const QString &key, const QString &defaultValue) const
{
    return content.value(key, defaultValue);
}

bool MessageContent::isValueSet(const QString &key) const
{
    return content.contains(key);
}

void MessageContent::setValue(const QString &key, const QString &value)
{
    content.insert(key, value);
}

void MessageContent::removeValue(const QString &key)
{
    content.remove(key);
}

QList<MessageContent> MessageContent::list(const QString &key) const
{
    return lists.values(key);
}

int MessageContent::listCount(const QString &key) const
{
    return lists.values(key).count();
}

void MessageContent::addToList(const QString &key, const MessageContent &item)
{
    lists.insert(key, item);
}

void MessageContent::removeList(const QString &key)
{
    lists.remove(key);
}

}
}
}
