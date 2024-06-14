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


#ifndef MESSAGECONTENT_H
#define MESSAGECONTENT_H

#include <QString>
#include <QMap>
#include <QMultiMap>

namespace Tucuxi {
namespace Gui {
namespace Rest {


class MessageContent
{

public:
    MessageContent();
    ~MessageContent();

    QString value(const QString &key, const QString &defaultValue = QString()) const;
    bool isValueSet(const QString &key) const;
    void setValue(const QString &key, const QString &value);
    void removeValue(const QString &key);

    QList<MessageContent> list(const QString &key) const;
    int listCount(const QString &key) const;
    void addToList(const QString &key, const MessageContent &item);
    void removeList(const QString &key);

private:
    QMap<QString, QString> content;
    QMultiMap<QString, MessageContent> lists;
};

}
}
}

#endif // MESSAGECONTENT_H
