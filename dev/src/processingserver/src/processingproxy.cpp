//@@license@@

#include "processingproxy.h"
#include "processingrequest.h"
#include "processingresponse.h"
#include "networkutils.h"

#include <QTcpSocket>
#include <iostream>

#define CONNECT(a,b,c,d) \
    if (!connect(a,b,c,d))\
            {\
                QString mess;\
                mess =QString("Signal connection error in file %1, line %2. %3: %4::%5").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(#a).arg(#b);\
                std::cout << qPrintable(mess) << std::endl;\
            }


ProcessingProxy::ProcessingProxy() : ProcessingInterface()
{
    m_socket=new QTcpSocket();
    m_responseTimeout = 10000; // 10 seconds
    CONNECT(m_socket, SIGNAL(hostFound()), this, SLOT(found()));
    CONNECT(m_socket, SIGNAL(connected()), this, SLOT(connected()));
    CONNECT(m_socket, SIGNAL(disconnected()), this, SLOT(looseConnection()));
    m_timer=new QTimer(this);
    m_timer->setInterval(1000);
    CONNECT(m_timer, SIGNAL(timeout()), this, SLOT(connected()));
}


void ProcessingProxy::printMessage(QString message)
{
    std::cout << qPrintable(message) << std::endl;
}


int ProcessingProxy::points(const SharedDrugResponseAnalysis analysis, const PointsTraits traits, DataSet &dataset)
{
    ProcessingRequest req;
    req.analysis = analysis;
    ProcessingResponse resp;
    process(req,resp);
    dataset = resp.dataset;
}

int ProcessingProxy::point(const SharedDrugResponseAnalysis analysis, const PointTraits traits, Amount &amount)
{
    qFatal("This method is not yet implemented");
    return -1;
}

int ProcessingProxy::percentiles(const SharedDrugResponseAnalysis analysis, const PercentilesTraits traits, QMap<double,DataSet> &concentrations)
{
    qFatal("This method is not yet implemented");
    return -1;
}

//int ProcessingProxy::point(const SharedPrediction prediction, const QDateTime &moment, Amount &amount)
//{
//    qFatal("This method is not yet implemented");
//    return -1;
//}

int ProcessingProxy::peak(const SharedDrugResponseAnalysis analysis, const PeakTraits traits, Amount &amount)
{
    qFatal("This method is not yet implemented");
    return -1;
}


//int ProcessingProxy::peak(const SharedPrediction _pred, const Duration &peakTime, Amount &amount)
//{
//    qFatal("This method is not yet implemented");
//    return -1;
//}

bool ProcessingProxy::process(const ProcessingRequest &request, ProcessingResponse &response)
{
    PREPAREBLOCK(COMMAND_REQUEST);
    out << request;
    ENDBLOCK
    sendBlock(block);
    if (m_socket->waitForReadyRead(m_responseTimeout))
    {
        getResponse(response);
        printMessage("Response received");
    }
    else
        printMessage("no data");
    return true;

}

#include <iostream>

bool ProcessingProxy::checkServerVersion()
{
    PREPAREBLOCK(COMMAND_REQ_VERSION);
    ENDBLOCK
    sendBlock(block);


    if (m_socket->waitForReadyRead(m_responseTimeout))
    {
    int Timeout=1000;
    while(!m_socket->atEnd())
    {
//        printMessage("data received");
        while(m_socket->bytesAvailable()<(int)sizeof(blocksize_t)) {
            if (!m_socket->waitForReadyRead(Timeout)) {
                //emit error(m_socket->error(), m_socket->errorString());
                return false;
            }
        }
        blocksize_t blockSize;

        QDataStream stream(m_socket);
        stream.setVersion(DATASTREAMVERSION);
        stream >> blockSize;

        while (m_socket->bytesAvailable() < blockSize) {
            if (!m_socket->waitForReadyRead(Timeout)) {
                //emit error(m_socket->error(), m_socket->errorString());
                return false;
            }
        }

        int command;
        stream >> command;
        switch(command)
        {
        case COMMAND_RESP_VERSION: // Read data corresponding...
        {
            QString version;
            stream >> version;
            std::cout << qPrintable(version) << std::endl;
            if (version.compare(SERVERVERSION) != 0)
            {
                printMessage("Server version does not match");
                return false;
            }
            return true;
         }
            break;
        default:
            stream.device()->readAll();
            return false;
            break;
        }
    }
    }
    return false;
}


ProcessingProxy::~ProcessingProxy()
{
    disconnect();
    m_socket->disconnect();
    m_socket->close();
    delete m_socket;
    delete m_timer;
}

void ProcessingProxy::looseConnection()
{
    emit disconnected();
}

int ProcessingProxy::connectToHost(QHostAddress address,int port)
{
//	m_timeout=timeout;
    m_address=address;
    if (port==0)
        m_port=DEFAULTPORT;
    else
        m_port=port;

    m_timer->start();
    CONNECT(m_socket, SIGNAL(hostFound()), this, SLOT(found()));
    m_socket->connectToHost(  m_address,m_port);
    m_state=WAITFORCONNECTION;
    return false;
}


int ProcessingProxy::connectToHostBlocked(QHostAddress address,int timeout,int port)
{
    m_address=address;
    if (port==0)
        m_port=DEFAULTPORT;
    else
        m_port=port;
    m_socket->connectToHost(  m_address,m_port);
    if (m_socket->waitForConnected(timeout))
    {
    //	connected();
        return true;
    }
    return false;
}


void ProcessingProxy::found()
{
    printMessage("Host found.");
//	m_foundHost=true;
    printMessage("Waiting for server connection...");
    QObject::disconnect(m_socket,SIGNAL(hostFound()),0,0);
}

void ProcessingProxy::connectionTimeout()
{
    if (m_socket->state()==QAbstractSocket::ConnectedState)
        return;
    printMessage("Connection timeout");
    emit connectTimeout();
}

void ProcessingProxy::connected()
{
    int connected=false;
    if (m_socket->waitForConnected(0))
    {
        m_timer->stop();
        connected=true;
        printMessage("Connected to remote server");

        // These lines should be decommented if working asynchronously. Else no
    //    CONNECT(m_socket,SIGNAL(readyRead()),
    //            this,SLOT(dataReady()))

        emit newConnection();
    }
    else
    {
        printMessage("...");
        m_socket->connectToHost(  m_address,m_port);
    }
}

int ProcessingProxy::getResponse(ProcessingResponse &response)
{
    int Timeout=1000;
    while(!m_socket->atEnd())
    {
//        printMessage("data received");
        while(m_socket->bytesAvailable()<(int)sizeof(blocksize_t)) {
            if (!m_socket->waitForReadyRead(Timeout)) {
                //emit error(m_socket->error(), m_socket->errorString());
                return false;
            }
        }
        blocksize_t blockSize;

        QDataStream stream(m_socket);
        stream.setVersion(DATASTREAMVERSION);
        stream >> blockSize;

        while (m_socket->bytesAvailable() < blockSize) {
            if (!m_socket->waitForReadyRead(Timeout)) {
                //emit error(m_socket->error(), m_socket->errorString());
                return false;
            }
        }

        int command;
        stream >> command;
        switch(command)
        {
        case COMMAND_RESPONSE: // Read data corresponding...
        {
            stream >> response;
            return true;
         }
            break;
        default:
            stream.device()->readAll();
            return false;
            break;
        }
    }
}

void ProcessingProxy::dataReady()
{
    ProcessingResponse response;
    getResponse(response);
}


void ProcessingProxy::Connection()
{


}

int ProcessingProxy::sendBlock(const QByteArray &array)
{
    if (m_socket->state()==QAbstractSocket::ConnectedState)
    {
        m_socket->write(array);
        return true;
    }
    else
    {
//            QMessageBox::warning(0, "Tcp Connection",
//                   "The Tcp connection to the simulation is not ready");
    }
    return false;
}


int ProcessingProxy::sendCommand(int command)
{
    if (m_state!=WAITFORCOMMAND)
        return false;
    PREPAREBLOCK(command);
    ENDBLOCK
    CLIENTSENDBLOCK;
    return true;
}


int ProcessingProxy::disconnectAll()
{
    //QMessageBox::warning(0,"","dics");
    return 1;//sendCommand(COMMAND_DISCONNECT_ALL);
}

