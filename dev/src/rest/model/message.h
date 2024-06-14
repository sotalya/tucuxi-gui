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


#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QTextStream>
#include <QDomDocument>

namespace Tucuxi {
namespace Gui {
namespace Rest {

class Message : public QObject
{
    Q_OBJECT

public:
    explicit Message(QObject *parent = nullptr);
    virtual ~Message();

    virtual void setData(QByteArray data) = 0;
    virtual QByteArray data() const = 0;

    virtual bool isValid() = 0;

    QString errorString() const;

protected:
    void setErrorString(const QString &errorString);
    void appendErrorString(const QString &errorString);

private:
    QString _errorString;
};

}
}
}

#endif // MESSAGE_H
