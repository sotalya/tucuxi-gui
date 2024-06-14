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


#include <QDateTime>
#include <QDataStream>

#include "tcpclient.h"
#include "tcpcommands.h"

TcpClient::TcpClient()
{
    socket = 0;
}

bool TcpClient::initialize()
{
    socket = new QTcpSocket(this);
    int timeout = 5000; // 5 seconds
    socket->connectToHost("localhost",DEFAULTPORT);
    if (socket->waitForConnected(timeout))
    {
        return true;
    }
    return false;
}

void TcpClient::close()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}

void TcpClient::sendMeasure(const QString &firstName,
                            const QString &lastName,
                            const QString &medicId,
                            const QDateTime t,
                            qreal concentration)
{
    PREPAREBLOCK(COMMAND_NEWMEASURE);
    out << firstName;
    out << lastName;
    out << medicId;
    out << t;
    out << concentration;
    ENDBLOCK;
    SENDBLOCK(socket);
}

void TcpClient::sendDose(const QString &firstName,
                         const QString &lastName,
                         const QString &medicId,
                         const QDateTime t,
                         qreal dose,
                         qreal interval)
{
    PREPAREBLOCK(COMMAND_NEWDOSE);
    out << firstName;
    out << lastName;
    out << medicId;
    out << t;
    out << dose;
    out << interval;
    ENDBLOCK;
    SENDBLOCK(socket);
}
