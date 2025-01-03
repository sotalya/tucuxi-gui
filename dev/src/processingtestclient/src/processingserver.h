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


#ifndef PROCESSINGSERVER_H
#define PROCESSINGSERVER_H

#include <QObject>

#include "processingmath.h"
#include "processingclient.h"

class ProcessingServer : public QObject
{
    Q_OBJECT
public:
    explicit ProcessingServer(QObject *parent = 0);
    virtual ~ProcessingServer();

    void initialize();

    int readAndExecuteCommand(QTcpSocket *socket,QDataStream &stream,ProcessingClient *client);

    QTcpServer m_server;
    QList<ProcessingClient *> m_clients;
    ProcessingMath *m_math;

    int m_port;

    void getCommand(QDataStream &stream, ProcessingClient *client);

signals:

public slots:
    void Connection();
    virtual void dataReady();
    void looseConnection(ProcessingClient *client);

};

#endif // PROCESSINGSERVER_H
