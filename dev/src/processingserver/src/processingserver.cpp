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


#include "processingserver.h"
#include "networkutils.h"
#include "processingrequest.h"
#include "processingresponse.h"

#include <QTcpSocket>

#define GENERALMESSAGE(mess) {std::cout << mess << std::endl;}

#include <iostream>

#define CONNECT(a,b,c,d) \
    if (!connect(a,b,c,d))\
            {\
                QString mess;\
                mess =QString("Signal connection error in file %1, line %2. %3: %4::%5").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(#a).arg(#b);\
                std::cout << qPrintable(mess) << std::endl;\
            }



ProcessingServer::ProcessingServer(QObject *parent) :
    QObject(parent)
{
    m_port = DEFAULTPORT;
    m_math = new ProcessingMath();
}

ProcessingServer::~ProcessingServer()
{
    delete m_math;
}


void ProcessingServer::initialize()
{
    m_server.listen(QHostAddress::Any,m_port);

    int port=m_server.serverPort();

    GENERALMESSAGE("*************************************************")
    GENERALMESSAGE("Server started")
    GENERALMESSAGE("Server port:\t\t\t"  << port)
    GENERALMESSAGE("Server version:\t\t\t" << qPrintable(SERVERVERSION))
    GENERALMESSAGE("*************************************************")

    CONNECT(&m_server, SIGNAL(newConnection()), this, SLOT(Connection()))
}


void ProcessingServer::looseConnection(ProcessingClient *client)
{
    bool cont=true;
    while (cont)
    {
        cont=false;
        for (int i = 0; i < m_clients.size(); ++i)
        {
            if (m_clients[i]==client)
                //            if (m_clients[i]->m_socket->state()==QAbstractSocket::UnconnectedState)
            {
                //        cont=true;
                GENERALMESSAGE("*************************************************")
                        GENERALMESSAGE("Disconnect a client")
                        GENERALMESSAGE("*************************************************")
                        ProcessingClient *client=m_clients[i];
                m_clients.removeAt(i);

                i=m_clients.size()+1;
                delete client;
            }
        }
    }
}


void ProcessingServer::Connection()
{
    QTcpSocket *socket=m_server.nextPendingConnection();

    GENERALMESSAGE("*************************************************")
    GENERALMESSAGE("New client: " << qPrintable(socket->peerAddress().toString()) )
    GENERALMESSAGE("*************************************************")

    ProcessingClient *newClient=new ProcessingClient();
    QDataStream *stream=new QDataStream();
    stream->setVersion(DATASTREAMVERSION);

    //    stream->setByteOrder(QDataStream::BigEndian);

    newClient->m_socket=socket;
    stream->setDevice(socket);
    m_clients.append(newClient);

    CONNECT(socket,SIGNAL(readyRead()),
            this,SLOT(dataReady()))

    CONNECT(socket,SIGNAL(disconnected()),
            newClient,SLOT(looseConnection()))

    CONNECT(newClient,SIGNAL(disconnected(ProcessingClient *)),
            this,SLOT(looseConnection(ProcessingClient *)))
}


void ProcessingServer::getCommand(QDataStream &stream, ProcessingClient *client)
{
    ProcessingRequest req;
    ProcessingResponse resp;
    stream >> req;
    GENERALMESSAGE("-------------------------------------")
    GENERALMESSAGE("New request")
//    GENERALMESSAGE("Nb points: " << req.prediction->spec()->nbPoints())
    GENERALMESSAGE("-------------------------------------")
//    m_math->points(req.analysis,resp.dataset);
    PREPAREBLOCK(COMMAND_RESPONSE)
    out << resp;
    ENDBLOCK;
    SENDBLOCK(client->m_socket);
    GENERALMESSAGE("Response sent back")
}

void ProcessingServer::returnVersion(QDataStream &stream, ProcessingClient *client)
{
    PREPAREBLOCK(COMMAND_RESP_VERSION);
    QString version(SERVERVERSION);
    out << version;
    ENDBLOCK;
    SENDBLOCK(client->m_socket);
    GENERALMESSAGE("Version sent back")
}

int ProcessingServer::readAndExecuteCommand(QTcpSocket *socket,QDataStream &stream,ProcessingClient *client)
{


    int Timeout=1000;


    while(socket->bytesAvailable()<(int)sizeof(blocksize_t)) {
        if (!socket->waitForReadyRead(Timeout)) {
            //emit error(m_socket->error(), m_socket->errorString());
            return -1;
        }
    }


    blocksize_t blockSize;
    stream.setDevice(socket);
//	stream.setVersion(QDataStream::Qt_4_0);

    stream >> blockSize;

    while (socket->bytesAvailable() < blockSize) {
        if (!socket->waitForReadyRead(Timeout)) {
            //emit error(m_socket->error(), m_socket->errorString());
            return -1;
        }
    }

    if (!client)
        return -1;

    int command;
    stream >> command;

//	cout << "Length: " << blockSize << " Command: " << command << endl;


    switch(command)
    {
    case COMMAND_REQUEST: GENERALMESSAGE("Command request")
                getCommand(stream,client);
        break;
    case COMMAND_REQ_VERSION :
                returnVersion(stream, client);
                break;
        default:
        return command;
        break;
    }
    return -1;
}


void ProcessingServer::dataReady()
{
    static int shouldsend=false;
    QTcpSocket *socket=dynamic_cast<QTcpSocket *>(sender());
    if (!socket)
        return;
    while(socket->bytesAvailable()>0)
    {
        int command;
        QDataStream stream;
        ProcessingClient *client;

        client=0;
        for(int i=0;i<m_clients.size();i++)
            if (m_clients[i]->m_socket==socket)
                client=m_clients[i];

        if ((command=readAndExecuteCommand(socket,stream,client))>0)
        {

            switch(command)
            {
            case 10: // Do something here, but actually with with implemtation we shouldn't
                break;
                default:
                    //TRACEERROR(TR_DATAREADY,"(): command default: " << command)
                break;
            }
        }
    }
}

