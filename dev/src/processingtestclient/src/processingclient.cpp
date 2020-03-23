#include "processingclient.h"

#include <QTcpSocket>

ProcessingClient::ProcessingClient(QObject *parent) :
    QObject(parent)
{
    m_socket=0;
}

ProcessingClient::~ProcessingClient()
{
    if (m_socket)
        m_socket->deleteLater();
}

void ProcessingClient::looseConnection()
{
    emit disconnected(this);
}
